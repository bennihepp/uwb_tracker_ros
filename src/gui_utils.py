import pyqtgraph


class MainWindow(pyqtgraph.GraphicsWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

    def showEvent(self, event):
        super(MainWindow, self).showEvent(event)

    def closeEvent(self, event):
        super(MainWindow, self).closeEvent(event)
