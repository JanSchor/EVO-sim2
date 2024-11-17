
# importing libs
import dearpygui.dearpygui as dpg
import tkinter as tk
from tkinter import filedialog
import time
import os.path

# Classes
class FileProcessingError(Exception):
    """Exception raised for errors in the file processing."""
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

class GenerationData:
    def __init__(self, path: str) -> None:
        """Init for GenerationData class"""
        if not path:
            self.setEmpty() # Called for default values
            return
        self.path = path
        self.name = path.split("/")[-1]
        self.version = ""
        self.headDict = {"gen": "N/A"}
        genNum = ""
        for char in range(len(self.name)-5, 0, -1):
            if self.name[char].isdigit():
                genNum = self.name[char] + genNum
            else:
                break
        if genNum:
            self.headDict["gen"] = genNum
        self.footDict = {"ave": "N/A"}
        self.grid = [None, None]
        self.safeZone = []
        self.wall = []
        self.records = []
        self.sqSize = 0
        self.offset = 0
        generationFile = open(path, 'r')
        while True:
            line = generationFile.readline().strip("\n")
            if not line:
                break
            pos = line.find("{")
            lineKey = line[:pos]
            lineVal = line[pos+1:-1]
            if self.version == "" and lineKey != "head":
                raise FileProcessingError("Expected header with version as first line on input file!")
            match lineKey:
                case "head":
                    self.setHead(lineVal)
                case "grid":
                    self.setGrid(lineVal)
                case "safe":
                    self.setSafe(lineVal)
                case "wall":
                    self.setWall(lineVal)
                case "foot":
                    self.setFoot(lineVal)
                case _:
                    if lineKey[:3] == "pos":
                        self.setPos(lineVal)
        generationFile.close()
        workWithGraphFile(self.name[0:13])


    def setHead(self, lineVal: str) -> None:
        """Sets some head values for GenerationData class"""
        values = lineVal[:-1].split(";")
        for val in values:
            splitVal = val.split(":")
            self.headDict[splitVal[0]] = splitVal[1]
        if "ver" in self.headDict.keys():
            self.version = self.headDict["ver"]

    def setFoot(self, lineVal: str) -> None:
        """Sets some foot values for GenerationData class"""
        values = lineVal[:-1].split(";")
        for val in values:
            splitVal = val.split(":")
            self.footDict[splitVal[0]] = splitVal[1]

    def setGrid(self, lineVal: str) -> None:
        """Sets grid values for GenerationData class"""
        lineVal.strip(";")
        values = lineVal[:-1].split(",")
        self.grid[0] = int(values[0])
        self.grid[1] = int(values[1])
        self.sqSize = 800//int(values[0])
        self.offset = 800%self.grid[0]

    def setSafe(self, lineVal: str) -> None:
        """Sets safe zone and criteria for GenerationData class"""
        if lineVal == "":
            return
        values = lineVal[:-1].split(";")
        for value in values:
            self.safeZone.append(value.split(","))
        for sz in self.safeZone:
            for cord in range(4):
                sz[cord] = int(sz[cord])

    def setWall(self, lineVal: str) -> None:
        """Sets wall values"""
        values = lineVal[:-1].split(";")
        for value in values:
            self.wall.append(value.split(","))
        for wa in self.wall:
            for cord in range(4):
                wa[cord] = int(wa[cord])

    def setPos(self, lineVal: str) -> None:
        """Created dict with creature positions for each step and appends records in GenerationData class"""
        values = lineVal[:-1].split(";")
        stepDict = {}
        for value in values:
            key, val = value.split(":")
            val = val.split(",")
            for valI in range(2):
                val[valI] = int(val[valI])
            stepDict[int(key)] = val
        self.records.append(stepDict)

    def setEmpty(self) -> None:
        """Sets GenerationData to default values"""
        self.path = None
        self.name = "None"
        self.version = "None"
        self.headDict = {"gen": "N/A"}
        self.footDict = {"ave":"N/A"}
        self.grid = [30, 30]
        self.safeZone = []
        self.wall = []
        self.records = []
        self.sqSize = 0
        self.offset = 800%self.grid[0]
        


# functions
def updateGUI() -> None:
    """Updates GUI pos and width, responsivenes"""
    dpg.configure_item("lf_btn", pos=(840-ld.offset, 40), width=240+ld.offset)
    dpg.configure_item("fl_txt", pos=(840-ld.offset, 95))
    dpg.set_value("fl_txt", "File loaded:\n"+ld.name)
    dpg.configure_item("sa_btn", pos=(840-ld.offset, 140), width=240+ld.offset)
    dpg.configure_item("at_inp", pos=(840-ld.offset, 200), width=100+ld.offset)
    dpg.configure_item("bo_lne", p1=(830-ld.offset, 220), p2=(1075, 220))
    dpg.configure_item("ge_txt", pos=(840-ld.offset, 260))
    dpg.set_value("ge_txt", "Generation: "+ld.headDict["gen"])
    dpg.configure_item("ca_txt", pos=(840-ld.offset, 280))
    dpg.set_value("ca_txt", "Creatures alive: "+ld.footDict["ave"])

    dpg.configure_item("lfg_btn", pos=(840-ld.offset, 650), width=240+ld.offset)
    dpg.configure_item("flg_txt", pos=(840-ld.offset, 705))
    dpg.configure_item("sg_btn", pos=(840-ld.offset, 750), width=240+ld.offset)

    
    dpg.delete_item("simArea")
    createSimArea()

def updateGraphGUI():
    pass

def createSimArea(step = 0) -> None:
    """Creates drawlist for animation"""
    with dpg.drawlist(width=820, height=820, pos=(0, 0), parent="animWindow", tag="simArea"):
        dpg.draw_rectangle((10, 10), (811-ld.offset, 811-(800%ld.grid[1])), color=(255, 255, 255))
        drawSafeZone()
        drawWall()
        if ld.name != "None":
            drawCreatures(step)

def drawGrid(gridX: int, gridY: int) -> None:
    """Shows grid of given size in animation canvas"""
    for yPos in range(1, gridY):
        dpg.draw_line(
            (10, 10+yPos*(800//gridY)),
            (810-(800%gridY), 10+yPos*(800//gridY)),
            color=(255, 255, 255, 100))
    for xPos in range(1, gridX):
        dpg.draw_line((10+xPos*(800//gridX), 10), (10+xPos*(800//gridX), 810-(800%gridX)), color=(255, 255, 255, 50))

def drawSafeZone() -> None:
    """Draws safe zone(s)"""
    for sz in ld.safeZone:
        dpg.draw_rectangle((10+sz[0]*ld.sqSize, 10+sz[1]*ld.sqSize), (10+(sz[2]+1)*ld.sqSize, 10+(sz[3]+1)*ld.sqSize), fill=(0, 255, 0, 30))

def drawWall() -> None:
    """Draws wall(s)"""
    for wa in ld.wall:
        dpg.draw_rectangle((10+wa[0]*ld.sqSize, 10+wa[1]*ld.sqSize), (10+(wa[2]+1)*ld.sqSize, 10+(wa[3]+1)*ld.sqSize), fill=(255, 255, 255))

def drawCreatures(step: int) -> None:
    """Draws all the creatures for specific step"""
    for c in ld.records[step].keys():
        cPos = ld.records[step][c]
        dpg.draw_circle((cPos[0]*ld.sqSize+10+(ld.sqSize//2), cPos[1]*ld.sqSize+10+(ld.sqSize//2)), ld.sqSize//2, color=(255, 0, 0), fill=(255, 0, 0, 150))


def selectFile() -> str:
    """Opens file explorer and returns path to selected txt file."""
    root = tk.Tk()
    root.withdraw()
    filePath = filedialog.askopenfilename(
        title="Select a file",
        filetypes=(("Text files", "*.txt"), ("All files", "*.*"))
    )
    return filePath

def workWithSimFile() -> None:
    """Takes selected file path and loads animation."""
    filePath = selectFile()
    if not filePath:
        return
    global ld
    ld = GenerationData(filePath)
    updateGUI()

def workWithGraphFile(*args) -> None:
    """Takes path from selected file and sets it to graphPath global var."""
    global graphPath
    if len(args) == 1:
        tempPath = "./exports/graph_logs/"+args[0]+"_graph.txt"
        if not os.path.isfile(tempPath):
            return
        # dialog to load file if found
    else:
        tempPath = selectFile()
        if not tempPath:
            return
    graphPath = tempPath
    fileName = graphPath.split("/")[-1]
    dpg.set_value("flg_txt", "Graph file loaded:\n"+fileName)


def createGraphWin() -> None:
    """Takes currently loaded path and creates graph out of that file."""
    if not graphPath:
        return
    
    fileName = graphPath.split("/")[-1]
    if dpg.does_alias_exist(fileName):
        dpg.delete_item(fileName)

    graphFile = open(graphPath, 'r')
    if not graphFile:
        return
    
    version = ""
    aliveDataX = []
    aliveDataY = []
    headDictGraph = {"ver":"N/A"}
    
    while True:
        line = graphFile.readline().strip("\n")
        if not line:
            break
        pos = line.find("{")
        lineKey = line[:pos]
        lineVal = line[pos+1:-1]
        if version == "" and lineKey != "head":
            raise FileProcessingError("Expected header with version as first line on input file!")
        match lineKey:
            case "head": # Head line
                values = lineVal[:-1].split(";")
                for val in values:
                    splitVal = val.split(":")
                    headDictGraph[splitVal[0]] = splitVal[1]
                if "ver" in headDictGraph.keys():
                    version = headDictGraph["ver"]

            case "alive": # Creatures alive values
                values = lineVal[:-1].split(";")
                for val in values:
                    splitVal = val.split(":")
                    aliveDataX.append(int(splitVal[0]))
                    aliveDataY.append(int(splitVal[1]))

    with dpg.window(label=fileName, width=1000, height=600, pos=(400, 200), no_resize=True, tag=fileName):
        with dpg.plot(label="Line Series", height=550, width=-1):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="Generation")
            with dpg.plot_axis(dpg.mvYAxis, label="Creatures alive"):
                dpg.add_line_series(aliveDataX, aliveDataY, label="Data")

    graphFile.close()
    
def animate() -> None:
    """Calls needed functions to create the animation"""
    for animStep in range(len(ld.records)):
        dpg.delete_item("simArea")
        createSimArea(animStep)
        time.sleep(1/len(ld.records)*int(dpg.get_value("at_inp"))) # change this to not work with sleep, but with program loop instead

def main() -> None:
    dpg.create_context()
    dpg.create_viewport(title='Generation animation', width=1200, height=800)

    filePath = None
    global ld
    ld = GenerationData(filePath)

    global graphPath
    graphPath = None

    # Creating window for the animation
    with dpg.window(label="Animation", width=1100, height=860, no_resize=True, tag="animWindow", no_close=True):
        createSimArea()
        dpg.add_button(label="Load simulation file", pos=(840-ld.offset, 40), width=240+ld.offset, height=50, callback=workWithSimFile, tag="lf_btn")
        dpg.add_text("File loaded:\n"+ld.name, pos=(840-ld.offset, 95), tag="fl_txt")
        dpg.add_button(label="Start animation", pos=(840-ld.offset, 140), width=240+ld.offset, height=50, tag="sa_btn", callback=animate)
        dpg.add_input_int(label="Animation time (sec)", pos=(840-ld.offset, 200), width=100+ld.offset,
                          min_value=1, min_clamped=True, default_value=5, tag="at_inp")
        dpg.draw_line(p1=(830-ld.offset, 220), p2=(1075, 220), tag="bo_lne")
        dpg.add_text("Generation: "+ld.headDict["gen"], pos=(840-ld.offset, 260), tag="ge_txt")
        dpg.add_text("Creatures alive: "+ld.footDict["ave"], pos=(840-ld.offset, 280), tag="ca_txt")

        dpg.add_button(label="Load graph file", pos=(840-ld.offset, 650), width=240+ld.offset, height=50, callback=workWithGraphFile, tag="lfg_btn")
        dpg.add_text("Graph file loaded:\n"+ld.name, pos=(840-ld.offset, 705), tag="flg_txt")
        dpg.add_button(label="Show graph", pos=(840-ld.offset, 750), width=240+ld.offset, height=50, tag="sg_btn", callback=createGraphWin)


    updateGraphGUI()

    while dpg.is_dearpygui_running(): # Loop of the app - use to generate animation steps in future
        dpg.render_dearpygui_frame()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.maximize_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


if __name__ == "__main__":
    main()
