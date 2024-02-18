#include "usbd_hid.h"
#include <string.h>

#define KEYBOARD_DELAY 15
#define MAX_CHARACTERS 6


enum {
    MODIFIER_NONE = 0,
    MODIFIER_CTRL = 1,
    MODIFIER_SHIFT = 2,
    MODIFIER_ALT = 4,
    MODIFIER_GUI = 8
};

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct
{
    uint8_t MODIFIER;
    uint8_t RESERVED;
    uint8_t KEYCODE[MAX_CHARACTERS];
} keyboard_HID_t;

keyboard_HID_t letterToCode(char letter) {
    keyboard_HID_t keyboardData = {0, 0, {0, 0, 0, 0, 0, 0}};

    if ('1' <= letter && letter <= '9') {
        keyboardData.KEYCODE[0] = letter - '1' + 0x1E;
    } else if ('a' <= letter && letter <= 'z') {
        keyboardData.KEYCODE[0] = letter - 'a' + 0x04;
    } else if ('A' <= letter && letter <= 'Z') {
        keyboardData.KEYCODE[0] = letter - 'A' + 0x04;
        keyboardData.MODIFIER = MODIFIER_SHIFT;
    } else {
        switch (letter) {
            case '0':
                keyboardData.KEYCODE[0] = 0x27;
                break;
            case ' ':
                keyboardData.KEYCODE[0] = 0x2C;
                break;
            case '!':
                keyboardData.KEYCODE[0] = 0x1E;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '"':
                keyboardData.KEYCODE[0] = 0x34;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '#':
                keyboardData.KEYCODE[0] = 0x20;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '$':
                keyboardData.KEYCODE[0] = 0x21;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '%':
                keyboardData.KEYCODE[0] = 0x22;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '&':
                keyboardData.KEYCODE[0] = 0x24;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '\'':
                keyboardData.KEYCODE[0] = 0x34;
                break;
            case '(':
                keyboardData.KEYCODE[0] = 0x26;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case ')':
                keyboardData.KEYCODE[0] = 0x27;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '*':
                keyboardData.KEYCODE[0] = 0x25;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case '+':
                keyboardData.KEYCODE[0] = 0x30;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case ',':
                keyboardData.KEYCODE[0] = 0x36;
                break;
            case '-':
                keyboardData.KEYCODE[0] = 0x2D;
                break;
            case '.':
                keyboardData.KEYCODE[0] = 0x37;
                break;
            case '/':
                keyboardData.KEYCODE[0] = 0x38;
                break;
            case ':':
                keyboardData.KEYCODE[0] = 0x33;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;
            case ';':
                keyboardData.KEYCODE[0] = 0x33;
                break;

            case '<':
                keyboardData.KEYCODE[0] = 0x36;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '=':
                keyboardData.KEYCODE[0] = 0x2E;
                break;

            case '>':
                keyboardData.KEYCODE[0] = 0x37;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '?':
                keyboardData.KEYCODE[0] = 0x38;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '@':
                keyboardData.KEYCODE[0] = 0x1F;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '[':
                keyboardData.KEYCODE[0] = 0x2F;
                break;

            case '\\':
                keyboardData.KEYCODE[0] = 0x31;
                break;

            case ']':
                keyboardData.KEYCODE[0] = 0x30;
                break;

            case '^':
                keyboardData.KEYCODE[0] = 0x23;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '_':
                keyboardData.KEYCODE[0] = 0x2D;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '`':
                keyboardData.KEYCODE[0] = 0x35;
                break;

            case '{':
                keyboardData.KEYCODE[0] = 0x2F;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '|':
                keyboardData.KEYCODE[0] = 0x31;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '}':
                keyboardData.KEYCODE[0] = 0x30;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            case '~':
                keyboardData.KEYCODE[0] = 0x35;
                keyboardData.MODIFIER = MODIFIER_SHIFT;
                break;

            default:
                break;
        }
    }

    return keyboardData;
}

void sendRawCharacter(uint8_t character, uint8_t modifier) {
    keyboard_HID_t keyboardData = {0, 0, {0, 0, 0, 0, 0, 0}};

    keyboardData.KEYCODE[0] = character;
    keyboardData.MODIFIER = modifier;
    HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_RESET);
    USBD_HID_SendReport(&hUsbDeviceFS, &keyboardData, sizeof(keyboardData));

    HAL_Delay(KEYBOARD_DELAY);

    keyboardData.KEYCODE[0] = 0;
    keyboardData.MODIFIER = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, &keyboardData, sizeof(keyboardData));
    HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_SET);

    HAL_Delay(KEYBOARD_DELAY);
}

void sendAutoCharacter(char character) {
    keyboard_HID_t keyboardData = letterToCode(character);
    HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_RESET);
    USBD_HID_SendReport(&hUsbDeviceFS, &keyboardData, sizeof(keyboardData));

    HAL_Delay(KEYBOARD_DELAY);

    keyboardData.KEYCODE[0] = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, &keyboardData, sizeof(keyboardData));
    HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_SET);

    HAL_Delay(KEYBOARD_DELAY);
}

void sendAutoCharacters(char *characters) {
    for (size_t i = 0; i < strlen(characters); i++) {
        sendAutoCharacter(characters[i]);
    }
}

void commandAndEnter(char *command) {
    sendAutoCharacters(command);
    sendRawCharacter(0x28, 0);
    HAL_Delay(50);
}