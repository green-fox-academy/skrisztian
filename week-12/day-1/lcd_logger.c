#include "lcd_logger.h"

log_msg(uint8_t log_type, char* message)
{
	if (LOG_LEVEL >=  log_type) {
		
		switch (log_type) {
		case (ERROR):
			LCD_ErrLog(message);
			break;
		case (USER):
			LCD_UsrLog(message);
			break;
		case (DEBUG):
			LCD_DbgLog(message);
			break;
		default:
			LCD_DbgLog(message);
			break;
		}
	}
	
	return;
}
	