let item = ""
let remainder = 0
let power = 0
let nible = ""
let bit = 0
let binary_nible = ""
let binary_string = ""
let dec = 0
let mode = ""
let string = ""
let index = 0
input.onButtonPressed(Button.A, () => {
    basic.showString("Test OK")
    serial.writeLine("Test OK")
})
serial.onDataReceived(serial.delimiters(Delimiters.NewLine), () => {
    string = serial.readLine()
    basic.showString(string)
    if (string == "-B2D") {
        mode = string
    } else if (string == "-D2B") {
        mode = string
    } else if (string == "-H2B") {
        mode = string
    } else if (string == "-B2H") {
        mode = string
    } else {
        if (mode == "-B2D") {
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
        } else if (mode == "-D2B") {
            dec = parseInt(string)
            binary_string = ""
            if (dec == 0) {
                binary_string = "0"
            } else {
                while (dec > 0) {
                    remainder = dec % 2
                    dec = dec / 2
                    binary_string = "" + remainder + binary_string
                }
            }
            basic.showIcon(IconNames.SmallDiamond)
            serial.writeLine(binary_string)
            basic.showString(binary_string)
        } else if (mode == "-H2B") {
            binary_string = ""
            binary_nible = ""
            for (let index = 0; index <= string.length - 1; index++) {
                nible = string.charAt(index)
                if (nible == "a" || nible == "A") {
                    dec = 10
                } else if (nible == "b" || nible == "B") {
                    dec = 11
                } else if (nible == "c" || nible == "C") {
                    dec = 12
                } else if (nible == "d" || nible == "D") {
                    dec = 13
                } else if (nible == "e" || nible == "E") {
                    dec = 14
                } else if (nible == "f" || nible == "F") {
                    dec = 15
                } else {
                    dec = parseInt(nible)
                }
                if (dec == 0) {
                    item = "0000"
                } else {
                    while (dec > 0) {
                        remainder = dec % 2
                        dec = dec / 2
                        binary_nible = "" + remainder + binary_nible
                    }
                    if (binary_nible.length < 4) {
                        for (let i = 0; i < 4 - binary_nible.length; i++) {
                            binary_nible = "0" + binary_nible
                        }
                    }
                }
                binary_string = "" + binary_string + binary_nible
            }
            serial.writeLine(binary_string)
            basic.showIcon(IconNames.SmallDiamond)
            basic.showString(binary_string)
        }
    }
})