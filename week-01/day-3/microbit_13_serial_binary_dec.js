let power = 0
let bit = 0
let dec = 0
let string = ""
input.onButtonPressed(Button.A, () => {
    basic.showString("Test OK")
    serial.writeLine("Test OK")
})
serial.onDataReceived(serial.delimiters(Delimiters.NewLine), () => {
    string = serial.readLine()
    basic.showString(string)
    dec = 0
    for (let index = 0; index <= string.length - 1; index++) {
        bit = parseInt(string.charAt(index))
        if (bit != 0) {
            power = string.length - (index + 1)
            dec = dec + Math.pow(2, power)
        }
    }
    basic.showIcon(IconNames.SmallDiamond)
    serial.writeNumber(dec)
    serial.writeLine("")
    basic.showNumber(dec)
})