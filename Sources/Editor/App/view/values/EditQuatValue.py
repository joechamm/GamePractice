from PyQt5.QtWidgets import QLineEdit, QWidget, QHBoxLayout, QLabel
from PyQt5.QtCore import Qt
from PyQt5.Qt import QDoubleValidator

from .Utils import TextToFloat, SetFloatToLineEdit

def _convertToEualerAngles(x, y, z, w):
    return x, y, z

def _convertFromEualerAngles(x, y, z):
    return x, y, z, 0

class EditQuatValue(QWidget):
    def __init__(self, value):
        super().__init__()

        self._val = value

        self._rootLayout = QHBoxLayout()

        self._xLabel = QLabel("<b>X:</>")
        self._rootLayout.addWidget(self._xLabel)

        self._xLineEdit = QLineEdit()
        self._xLineEdit.textEdited.connect(self._signal_lineEdit_textEdited)
        self._xLineEdit.setAlignment(Qt.AlignRight)
        self._xLineEdit.setValidator(QDoubleValidator())
        self._rootLayout.addWidget(self._xLineEdit)

        self._yLabel = QLabel("<b>Y:</>")
        self._rootLayout.addWidget(self._yLabel)

        self._yLineEdit = QLineEdit()
        self._yLineEdit.textEdited.connect(self._signal_lineEdit_textEdited)
        self._yLineEdit.setAlignment(Qt.AlignRight)
        self._yLineEdit.setValidator(QDoubleValidator())
        self._rootLayout.addWidget(self._yLineEdit)

        self._zLabel = QLabel("<b>Z:</>")
        self._rootLayout.addWidget(self._zLabel)

        self._zLineEdit = QLineEdit()
        self._zLineEdit.textEdited.connect(self._signal_lineEdit_textEdited)
        self._zLineEdit.setAlignment(Qt.AlignRight)
        self._zLineEdit.setValidator(QDoubleValidator())
        self._rootLayout.addWidget(self._zLineEdit)

        self._rootLayout.setContentsMargins(1, 1, 1, 1)
        self.setLayout(self._rootLayout)

        self._pull()

    def _signal_lineEdit_textEdited(self, text):
        x = TextToFloat(self._xLineEdit.text())
        y = TextToFloat(self._zLineEdit.text())
        z = TextToFloat(self._yLineEdit.text())
        xVal, yVal, zVal, wVal = _convertFromEualerAngles(x, y, z)

        self._push(xVal, yVal, zVal, wVal)
        self._pull()

    def _pull(self):
        x, y, z, w = self._val.getVal()
        xAngle, yAngle, zAngle = _convertToEualerAngles(x, y, z, w)
        SetFloatToLineEdit(self._xLineEdit, xAngle)
        SetFloatToLineEdit(self._yLineEdit, yAngle)
        SetFloatToLineEdit(self._zLineEdit, zAngle)

    def _push(self, x, y, z, w):
        self._val.setVal(x, y, z, w)