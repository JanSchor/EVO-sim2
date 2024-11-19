
# importing libs
import dearpygui.dearpygui as dpg
import tkinter as tk
from tkinter import filedialog
import time
import os.path


# -------
# Classes
# -------

class FileProcessingError(Exception):
    """Exception raised for errors in the file processing."""
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

class GenerationData:
    """CLass for storing the generation data"""
    def __init__(self, path: str) -> None:
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
        self.graphWinTime = 0
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
            linesFunctionsDict = {
                "head": self.setHead,
                "grid": self.setGrid,
                "safe": self.setSafe,
                "wall": self.setWall,
                "foot": self.setFoot
                }
            if lineKey in linesFunctionsDict.keys():
                linesFunctionsDict.get(lineKey)(lineVal)
            if lineKey[:3] == "pos":
                self.setPos(lineVal)

        generationFile.close()
        
        if self.checkForGraphFile(self.name[0:13]) and self.name[0:13] != dpg.get_value("flg_txt")[19:32]:
            if dpg.does_alias_exist("file_found_win"):
                dpg.delete_item("file_found_win")
            if dpg.does_alias_exist("graph_window_text"):
                dpg.remove_alias("graph_window_text")
            self.graphWinTime = int(time.time())+15
            self.createGraphFoundWindow()

        animRun.currentStep = 0
        animRun.animRunning = False
        dpg.set_value("cs_txt", "Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)))
        resetAnimation()


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
        """Creates dict with creature positions for each step and appends records in GenerationData class"""
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
        self.graphWinTime = 0

    def checkForGraphFile(self, name) -> bool:
        """Checks if graph file with coresponding name exists"""
        tempPath = "./exports/graph_logs/"+name+"_graph.txt"
        if os.path.isfile(tempPath):
            return True
        return False
    
    def createGraphFoundWindow(self) -> None:
        """Creates popup window for loading graph dialog"""
        graphFoundWinWidth = 400
        graphFoundWinHeight = 100
        with dpg.window(label="", width=graphFoundWinWidth, height=graphFoundWinHeight,
                        pos=((viewportWidth//2)-(graphFoundWinWidth//2), 20),
                        no_resize=True, no_title_bar=True, no_move=True, no_close=True,
                        tag="file_found_win"):
            dpg.add_text("Graph file found for loaded animation. Load?", wrap=graphFoundWinWidth-20, tag="graph_window_text")
            dpg.add_button(label="Load", pos=(20, 55), width=70, height=30, callback=lambda:workWithGraphFile(self.name[0:13]))
            dpg.add_button(label="Cancel", pos=(100, 55), width=70, height=30, callback=lambda:dpg.delete_item("file_found_win"))

            dpg.bind_item_font("graph_window_text", "proggyClean")
    
class AnimRun():
    """CLass for setting some values to the animation"""
    def __init__(self) -> None:
        self.animRunning = False
        self.stepsPerSec = 5
        self.startAnimUnix = time.time()
        self.currentStep = 0
                

# ---------
# Functions
# ---------

def updateGUI() -> None:
    """Updates GUI pos and width, responsivenes"""
    dpg.configure_item("lf_btn", pos=(840-loadedData.offset, 40), width=240+loadedData.offset)
    dpg.configure_item("fl_txt", pos=(840-loadedData.offset, 95))
    dpg.set_value("fl_txt", "File loaded:\n"+loadedData.name)
    dpg.configure_item("sa_btn", pos=(840-loadedData.offset, 140), width=240+loadedData.offset)
    dpg.configure_item("ra_btn", pos=(840-loadedData.offset, 200), width=240+loadedData.offset)
    dpg.configure_item("at_inp", pos=(840-loadedData.offset, 260), width=100+loadedData.offset)
    dpg.configure_item("bo_lne", p1=(830-loadedData.offset, 280))
    dpg.configure_item("ge_txt", pos=(840-loadedData.offset, 320))
    dpg.set_value("ge_txt", "Generation: "+loadedData.headDict["gen"])
    dpg.configure_item("ca_txt", pos=(840-loadedData.offset, 340))
    dpg.set_value("ca_txt", "Creatures survived: "+loadedData.footDict["ave"])
    dpg.configure_item("cs_txt", pos=(840-loadedData.offset, 360))
    dpg.set_value("cs_txt", "Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)))

    dpg.configure_item("ms_txt", pos=(840-loadedData.offset, 390))
    dpg.configure_item("stepsBtnP1", pos=(840-loadedData.offset, 420))
    dpg.configure_item("stepsBtnP10", pos=(840-loadedData.offset+50, 420))
    dpg.configure_item("stepsBtnP50", pos=(840-loadedData.offset+100, 420))
    dpg.configure_item("stepsBtnN1", pos=(840-loadedData.offset, 450))
    dpg.configure_item("stepsBtnN10", pos=(840-loadedData.offset+50, 450))
    dpg.configure_item("stepsBtnN50", pos=(840-loadedData.offset+100, 450))

    dpg.configure_item("lfg_btn", pos=(840-loadedData.offset, 650), width=240+loadedData.offset)
    dpg.configure_item("flg_txt", pos=(840-loadedData.offset, 705))
    dpg.configure_item("sg_btn", pos=(840-loadedData.offset, 750), width=240+loadedData.offset)

    createSimArea()

def createSimArea(step = 0) -> None:
    """Creates drawlist for animation"""
    if dpg.does_alias_exist("simArea"):
        dpg.delete_item("simArea")
    with dpg.drawlist(width=820, height=820, pos=(0, 0), parent="animWindow", tag="simArea"):
        dpg.draw_rectangle((10, 10), (811-loadedData.offset, 811-(800%loadedData.grid[1])), color=(255, 255, 255))
        drawSafeZone()
        drawWall()
        if loadedData.name != "None":
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
    for sz in loadedData.safeZone:
        dpg.draw_rectangle((10+sz[0]*loadedData.sqSize, 10+sz[1]*loadedData.sqSize), (10+(sz[2]+1)*loadedData.sqSize, 10+(sz[3]+1)*loadedData.sqSize), fill=(0, 255, 0, 30))

def drawWall() -> None:
    """Draws wall(s)"""
    for wa in loadedData.wall:
        dpg.draw_rectangle((10+wa[0]*loadedData.sqSize, 10+wa[1]*loadedData.sqSize), (10+(wa[2]+1)*loadedData.sqSize, 10+(wa[3]+1)*loadedData.sqSize), fill=(255, 255, 255))

def drawCreatures(step: int) -> None:
    """Draws all the creatures for specific step"""
    if step >= len(loadedData.records):
        step = len(loadedData.records)-1
    for c in loadedData.records[step].keys():
        cPos = loadedData.records[step][c]
        dpg.draw_circle((cPos[0]*loadedData.sqSize+10+(loadedData.sqSize//2), cPos[1]*loadedData.sqSize+10+(loadedData.sqSize//2)), loadedData.sqSize//2, color=(255, 0, 0), fill=(255, 0, 0, 150))


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
    global loadedData
    loadedData = GenerationData(filePath)
    updateGUI()

def workWithGraphFile(*args) -> None:
    """Takes path from selected file and sets it to graphPath global var."""
    global graphPath
    if len(args) == 1:
        if dpg.does_item_exist("file_found_win"):
            dpg.delete_item("file_found_win")
        tempPath = "./exports/graph_logs/"+args[0]+"_graph.txt"
        if not os.path.isfile(tempPath):
            return
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
                dpg.add_line_series(aliveDataX, aliveDataY, label="Creatures survived")

    graphFile.close()
    
def animate() -> None:
    """Calls needed functions to start the animation"""
    if not loadedData.records:
        return
    animRun.animRunning = True
    animRun.startAnimUnix = time.time()
    animRun.stepsPerSec = int(dpg.get_value("at_inp"))
    animRun.currentStep = 0
    dpg.configure_item("sa_btn", label="Stop animation", callback=stopAnimation)

def stopAnimation() -> None:
    """Stops the animation"""
    animRun.animRunning = False
    dpg.configure_item("sa_btn", label="Continue animation", callback=continueAnimation)

def continueAnimation() -> None:
    """Resumes the animation"""
    animRun.animRunning = True
    animRun.stepsPerSec = int(dpg.get_value("at_inp"))
    animRun.startAnimUnix = time.time() - (animRun.currentStep / animRun.stepsPerSec)
    dpg.configure_item("sa_btn", label="Stop animation", callback=stopAnimation)

def resetAnimation() -> None:
    """Resets the animation"""
    animRun.animRunning = False
    animRun.currentStep = 0
    dpg.configure_item("sa_btn", label="Start animation", callback=animate)
    createSimArea(animRun.currentStep)
    dpg.set_value("cs_txt", "Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)))

def modStep(mod) -> None:
    """Modifies the current step by given modifyer"""
    currStep = animRun.currentStep
    currStep += mod
    if currStep < 0:
        currStep = 0
    elif currStep >= len(loadedData.records):
        currStep = len(loadedData.records)
    animRun.currentStep = currStep
    createSimArea(animRun.currentStep)
    dpg.set_value("cs_txt", "Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)))

def createAnimationWindow() -> None:
    """Creation of GUI for animation window"""
    with dpg.window(label="Animation", width=1100, height=860, tag="animWindow",
                        no_resize=True, no_close=True, no_bring_to_front_on_focus=True):
            createSimArea()
            dpg.add_button(label="Load simulation file", pos=(840-loadedData.offset, 40), width=240+loadedData.offset, height=50, callback=workWithSimFile, tag="lf_btn")
            dpg.add_text("File loaded:\n"+loadedData.name, pos=(840-loadedData.offset, 95), tag="fl_txt")
            dpg.add_button(label="Start animation", pos=(840-loadedData.offset, 140), width=240+loadedData.offset, height=50, tag="sa_btn", callback=animate)
            dpg.add_button(label="Reset animation", pos=(840-loadedData.offset, 200), width=240+loadedData.offset, height=50, tag="ra_btn", callback=resetAnimation)
            dpg.add_input_int(label="Steps per sec", pos=(840-loadedData.offset, 260), width=100+loadedData.offset,
                            min_value=1, min_clamped=True, default_value=60, tag="at_inp")
            dpg.draw_line(p1=(830-loadedData.offset, 280), p2=(1075, 280), tag="bo_lne")
            dpg.add_text("Generation: "+loadedData.headDict["gen"], pos=(840-loadedData.offset, 320), tag="ge_txt")
            dpg.add_text("Creatures survived: "+loadedData.footDict["ave"], pos=(840-loadedData.offset, 340), tag="ca_txt")
            dpg.add_text("Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)),
                         pos=(840-loadedData.offset, 360), tag="cs_txt")
            
            dpg.add_text("Manual steps:", pos=(840-loadedData.offset, 390), tag="ms_txt")
            dpg.add_button(label="+1", width=40, pos=(840-loadedData.offset, 420), tag="stepsBtnP1", callback=lambda:modStep(1))
            dpg.add_button(label="+10", width=40, pos=(840-loadedData.offset+50, 420), tag="stepsBtnP10", callback=lambda:modStep(10))
            dpg.add_button(label="+50", width=40, pos=(840-loadedData.offset+100, 420), tag="stepsBtnP50", callback=lambda:modStep(50))

            dpg.add_button(label="-1", width=40, pos=(840-loadedData.offset, 450), tag="stepsBtnN1", callback=lambda:modStep(-1))
            dpg.add_button(label="-10", width=40, pos=(840-loadedData.offset+50, 450), tag="stepsBtnN10", callback=lambda:modStep(-10))
            dpg.add_button(label="-50", width=40, pos=(840-loadedData.offset+100, 450), tag="stepsBtnN50", callback=lambda:modStep(-50))

            dpg.add_button(label="Load graph file", pos=(840-loadedData.offset, 650), width=240+loadedData.offset, height=50, callback=workWithGraphFile, tag="lfg_btn")
            dpg.add_text("Graph file loaded:\n"+loadedData.name, pos=(840-loadedData.offset, 705), tag="flg_txt")
            dpg.add_button(label="Show graph", pos=(840-loadedData.offset, 750), width=240+loadedData.offset, height=50, tag="sg_btn", callback=createGraphWin)



# -------------
# MAIN FUNCTION
# -------------

def main() -> None:
    """Main function"""

    # DPG init
    dpg.create_context()
    dpg.create_viewport(title='Generation animation', width=1200, height=800)

    # Creating registery for fonts
    with dpg.font_registry():
        dpg.add_font("./tools/fonts/ProggyCleanSZ.ttf", 16, tag="proggyClean")

    # Loading data from file path, default is now set to None
    filePath = None
    global loadedData
    loadedData = GenerationData(filePath)

    # Creating global varible for graph path and setting to None
    global graphPath
    graphPath = None

    # Creating global object for animation running settings
    global animRun
    animRun = AnimRun()

    # Animation window init
    createAnimationWindow()

    # Resolution
    global viewportWidth, viewportHeight
    viewportWidth = 1920
    viewportHeight = 1080

    # More DPG stuff
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.maximize_viewport()

    # DPG main rendering loop
    while dpg.is_dearpygui_running():
        if animRun.animRunning:
            currTime = time.time()
            currentStep = int((currTime - animRun.startAnimUnix)*animRun.stepsPerSec)
            if currentStep > len(loadedData.records):
                currentStep = len(loadedData.records)
                animRun.animRunning = False
            animRun.currentStep = currentStep
            dpg.set_value("cs_txt", "Current step: " + str(animRun.currentStep) + " / " + str(len(loadedData.records)))
            createSimArea(animRun.currentStep)
        
        if loadedData.graphWinTime < int(time.time()) and dpg.does_alias_exist("file_found_win"):
            dpg.delete_item("file_found_win")
        dpg.render_dearpygui_frame()

    # End of DPG stuff
    dpg.destroy_context()


# Starting the program
if __name__ == "__main__":
    main()
