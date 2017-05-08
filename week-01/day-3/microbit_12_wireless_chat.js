let string = ""
radio.setGroup(27)
input.onButtonPressed(Button.A, () => {
    serial.writeLine("Test OK")
    basic.showString("Test OK")
})
basic.forever(() => {
    basic.showString(string)
})
serial.onDataReceived(serial.delimiters(Delimiters.NewLine), () => {
    string = serial.readLine()
    radio.sendString(string)
})
radio.onDataPacketReceived(({receivedString}) => {
    string = receivedString
    serial.writeLine(string)
})