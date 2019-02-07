#
# german.scan.rb
# ScanCode translation for a german keyboard
#

theInputSystem = get('/sys/server/input');

if (theInputSystem == nil)
        print "(german.scan.rb) cannot find the InputServer)\n";
else
	theInputSystem.addCode(Input.IC_1, '1', '1'[0], '!'[0],   0);
	theInputSystem.addCode(Input.IC_2, '2', '2'[0], '@'[0],   0);
	theInputSystem.addCode(Input.IC_3, '3', '3'[0], '#'[0],   0);
	theInputSystem.addCode(Input.IC_4, '4', '4'[0], '$'[0],   0);
	theInputSystem.addCode(Input.IC_5, '5', '5'[0], '%'[0],   0);
	theInputSystem.addCode(Input.IC_6, '6', '6'[0], '^'[0],   0);
	theInputSystem.addCode(Input.IC_7, '7', '7'[0], '&'[0], '{'[0]);
	theInputSystem.addCode(Input.IC_8, '8', '8'[0], '*'[0], '['[0]);
	theInputSystem.addCode(Input.IC_9, '9', '9'[0], '('[0], ']'[0]);
	theInputSystem.addCode(Input.IC_0, '0', '0'[0], ')'[0], '}'[0]);
	# function keys
	theInputSystem.addCode(Input.IC_F1, 'f1', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F2, 'f2', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F3, 'f3', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F4, 'f4', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F5, 'f5', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F6, 'f6', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F7, 'f7', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F8, 'f8', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F9, 'f9', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F10, 'f10', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F11, 'f11', 0, 0, 0);
	theInputSystem.addCode(Input.IC_F12, 'f12', 0, 0, 0);
	# alphabet
	theInputSystem.addCode(Input.IC_A, 'a', 'a'[0], 'A'[0], 0);
	theInputSystem.addCode(Input.IC_B, 'b', 'b'[0], 'B'[0], 0);
	theInputSystem.addCode(Input.IC_C, 'c', 'c'[0], 'C'[0], 0);
	theInputSystem.addCode(Input.IC_D, 'd', 'd'[0], 'D'[0], 0);
	theInputSystem.addCode(Input.IC_E, 'e', 'e'[0], 'E'[0], 0);
	theInputSystem.addCode(Input.IC_F, 'f', 'f'[0], 'F'[0], 0);
	theInputSystem.addCode(Input.IC_G, 'g', 'g'[0], 'G'[0], 0);
	theInputSystem.addCode(Input.IC_H, 'h', 'h'[0], 'H'[0], 0);
	theInputSystem.addCode(Input.IC_I, 'i', 'i'[0], 'I'[0], 0);
	theInputSystem.addCode(Input.IC_J, 'j', 'j'[0], 'J'[0], 0);
	theInputSystem.addCode(Input.IC_K, 'k', 'k'[0], 'K'[0], 0);
	theInputSystem.addCode(Input.IC_L, 'l', 'l'[0], 'L'[0], 0);
	theInputSystem.addCode(Input.IC_M, 'm', 'm'[0], 'M'[0], 0);
	theInputSystem.addCode(Input.IC_N, 'n', 'n'[0], 'N'[0], 0);
	theInputSystem.addCode(Input.IC_O, 'o', 'o'[0], 'O'[0], 0);
	theInputSystem.addCode(Input.IC_P, 'p', 'p'[0], 'P'[0], 0);
	theInputSystem.addCode(Input.IC_Q, 'q', 'q'[0], 'Q'[0], 0);
	theInputSystem.addCode(Input.IC_R, 'r', 'r'[0], 'R'[0], 0);
	theInputSystem.addCode(Input.IC_S, 's', 's'[0], 'S'[0], 0);
	theInputSystem.addCode(Input.IC_T, 't', 't'[0], 'T'[0], 0);
	theInputSystem.addCode(Input.IC_U, 'u', 'u'[0], 'U'[0], 0);
	theInputSystem.addCode(Input.IC_V, 'v', 'v'[0], 'V'[0], 0);
	theInputSystem.addCode(Input.IC_W, 'w', 'w'[0], 'W'[0], 0);
	theInputSystem.addCode(Input.IC_X, 'x', 'x'[0], 'X'[0], 0);
	theInputSystem.addCode(Input.IC_Y, 'y', 'y'[0], 'Y'[0], 0);
	theInputSystem.addCode(Input.IC_Z, 'z', 'z'[0], 'Z'[0], 0);
	# keypad
	theInputSystem.addCode(Input.IC_KP0, 'kp0', '0'[0], '0'[0], 0);
	theInputSystem.addCode(Input.IC_KP1, 'kp1', '1'[0], '1'[0], 0);
	theInputSystem.addCode(Input.IC_KP2, 'kp2', '2'[0], '2'[0], 0);
	theInputSystem.addCode(Input.IC_KP3, 'kp3', '3'[0], '3'[0], 0);
	theInputSystem.addCode(Input.IC_KP4, 'kp4', '4'[0], '4'[0], 0);
	theInputSystem.addCode(Input.IC_KP5, 'kp5', '5'[0], '5'[0], 0);
	theInputSystem.addCode(Input.IC_KP6, 'kp6', '6'[0], '6'[0], 0);
	theInputSystem.addCode(Input.IC_KP7, 'kp7', '7'[0], '7'[0], 0);
	theInputSystem.addCode(Input.IC_KP8, 'kp8', '8'[0], '8'[0], 0);
	theInputSystem.addCode(Input.IC_KP9, 'kp9', '9'[0], '9'[0], 0);
	theInputSystem.addCode(Input.IC_KP_DECIMAL, 'kp_decimal',	'.'[0], '.'[0], 0);
	theInputSystem.addCode(Input.IC_KP_DIVIDE,  'kp_divide'	,	'/'[0], '/'[0], 0);
	theInputSystem.addCode(Input.IC_KP_MULTIPLY,'kp_multiply',	'*'[0], '*'[0], 0);
	theInputSystem.addCode(Input.IC_KP_MINUS,   'kp_minus',		'-'[0], '-'[0], 0);
	theInputSystem.addCode(Input.IC_KP_PLUS,    'kp_plus',		'+'[0], '+'[0], 0);
	theInputSystem.addCode(Input.IC_KP_ENTER,   'kp_enter',		  0,   0, 0);
	# arrows + home/end pad
	theInputSystem.addCode(Input.IC_UP,	        'up', 0, 0, 0);
	theInputSystem.addCode(Input.IC_DOWN,		'down', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LEFT,		'left', 0, 0, 0);
	theInputSystem.addCode(Input.IC_RIGHT,		'right', 0, 0, 0);
	theInputSystem.addCode(Input.IC_INSERT,		'insert', 0, 0, 0);
	theInputSystem.addCode(Input.IC_DELETE,		'delete', 0, 0, 0);
	theInputSystem.addCode(Input.IC_HOME,		'home', 0, 0, 0);
	theInputSystem.addCode(Input.IC_END,		'end', 0, 0, 0);
	theInputSystem.addCode(Input.IC_PAGEUP,		'pageup', 0, 0, 0);
	theInputSystem.addCode(Input.IC_PAGEDOWN,	'pagedown', 0, 0, 0);
	# key state modifier keys
	theInputSystem.addCode(Input.IC_NUMLOCK,	'numlock', 0, 0, 0);
	theInputSystem.addCode(Input.IC_CAPSLOCK,	'capslock', 0, 0, 0);
	theInputSystem.addCode(Input.IC_SCROLLOCK,	'scrollock', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LSHIFT,		'lshift', 0, 0, 0);
	theInputSystem.addCode(Input.IC_RSHIFT, 'rshift', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LCTRL,  'lctrl', 0, 0, 0);
	theInputSystem.addCode(Input.IC_RCTRL,  'rctrl', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LALT,   'lalt', 0, 0, 0);
	theInputSystem.addCode(Input.IC_RALT,   'ralt', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LSUPER, 'lsuper', 0, 0, 0);	
	theInputSystem.addCode(Input.IC_RSUPER, 'rsuper', 0, 0, 0);	
	# other keys ('cursor control', punctuation)
	theInputSystem.addCode(Input.IC_ESCAPE,    'escape', 0, 0, 0);
	theInputSystem.addCode(Input.IC_PRINT,     'print',  0, 0, 0);
	theInputSystem.addCode(Input.IC_PAUSE,     'pause', 0, 0, 0);
	theInputSystem.addCode(Input.IC_GRAVE,     '`', '`'[0], '~'[0], 0);
	theInputSystem.addCode(Input.IC_MINUS,     '-', '-'[0], '_'[0], 0);
	theInputSystem.addCode(Input.IC_EQUALS,    '=', '='[0], '+'[0], 0);
	theInputSystem.addCode(Input.IC_BACKSLASH, '\\', '\\'[0], '|'[0], 0);
	theInputSystem.addCode(Input.IC_BACKSPACE, 'backspace', 0, 0, 0);
	
	theInputSystem.addCode(Input.IC_TAB,      'tab', 0, 0, 0);
	theInputSystem.addCode(Input.IC_LBRACKET, '[', '['[0], '{'[0], 0);
	theInputSystem.addCode(Input.IC_RBRACKET, ']', ']'[0], '}'[0], 0);
	theInputSystem.addCode(Input.IC_RETURN,   'return', 0, 0, 0);
	
	theInputSystem.addCode(Input.IC_SEMICOLON, ';', ';'[0], ':'[0], 0);
	theInputSystem.addCode(Input.IC_APOSTROPHE, '\'', '\''[0], '"'[0], 0);
	
	theInputSystem.addCode(Input.IC_OEM_102, 'oem_102', 0, 0, 0);
	theInputSystem.addCode(Input.IC_COMMA,   ',', ','[0], '<'[0], 0);
	theInputSystem.addCode(Input.IC_PERIOD,  '.', '.'[0], '>'[0], 0);
	theInputSystem.addCode(Input.IC_SLASH,   '/', '/'[0], '?'[0], 0);
	
	theInputSystem.addCode(Input.IC_SPACE,   'space', ' '[0], ' '[0], 0);
	# mouse buttons
	theInputSystem.addCode(Input.IC_MOUSE_LEFT,    'mouse_left', 0, 0, 0);	
	theInputSystem.addCode(Input.IC_MOUSE_RIGHT,   'mouse_right', 0, 0, 0);	
	theInputSystem.addCode(Input.IC_MOUSE_MIDDLE, 'mouse_middle', 0, 0, 0);
	#mouse axis
	theInputSystem.addCode(Input.IC_AXISX, 'axisx', 0, 0, 0);
	theInputSystem.addCode(Input.IC_AXISY, 'axisy', 0, 0, 0);
	theInputSystem.addCode(Input.IC_AXISZ, 'axisz', 0, 0, 0);
	# timer
	theInputSystem.addCode(Input.IC_AXIST, 'axist', 0, 0, 0);
end
