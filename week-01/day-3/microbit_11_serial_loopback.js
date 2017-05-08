let string = ""
input.onButtonPressed(Button.A, () => {
    serial.writeLine("Hello!")
})
serial.onDataReceived(serial.delimiters(Delimiters.NewLine), () => {
    string = serial.readLine()
    basic.showString(string)
    serial.writeLine(string)
})