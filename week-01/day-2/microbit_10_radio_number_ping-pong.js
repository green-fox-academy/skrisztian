input.onButtonPressed(Button.A, () => {
    radio.sendNumber(1)
    basic.showIcon(IconNames.Yes)
})
radio.onDataPacketReceived(({receivedNumber}) => {
    basic.clearScreen()
    basic.showNumber(receivedNumber)
    basic.pause(2000)
    radio.sendNumber(receivedNumber + 1)
    basic.clearScreen()
    basic.showIcon(IconNames.Yes)
})
radio.setGroup(27)