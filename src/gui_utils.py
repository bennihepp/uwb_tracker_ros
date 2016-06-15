import numpy as np
import pyqtgraph

class MainWindow(pyqtgraph.GraphicsWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

    def showEvent(self, event):
        super(MainWindow, self).showEvent(event)

    def closeEvent(self, event):
        super(MainWindow, self).closeEvent(event)


class PlotData(object):

    def __init__(self, plot, max_data_length=None):
        self.plot = plot
        self.curves = []
        self.data = []
        self.max_data_length = max_data_length

    def add_curve(self, pen, initial_data=None, **kwargs):
        self.curves.append(self.plot.plot(pen=pen, **kwargs))
        if initial_data is None:
            if self.max_data_length is None:
                initial_data = []
            else:
                initial_data = np.zeros((self.max_data_length,))
        self.data.append(initial_data)

    def add_point(self, index, value):
        assert(index < len(self.curves))
        if self.max_data_length is None:
            self.data[index].append(value)
        else:
            self.data[index][:-1] = self.data[index][1:]
            self.data[index][-1] = value
            if len(self.data[index]) > self.max_data_length:
                self.data[index] = self.data[index][-self.max_data_length:len(self.data[index])]
        self.curves[index].setData(self.data[index])

    def get_plot(self):
        return self.plot

    def __len__(self):
        return len(self.curves)
