TITLE Low-level I/O Procedures     (SullivanJoseph_Program5A.asm)

; Author: Joseph Sullivan
; CS271-400 / Assignment 5		Date: 8/7/2017
; Description: Program gets 10 integers from the user and stores them in an array.
;			   Then, the array's integers, sum, and average are displayed.

INCLUDE Irvine32.inc

; Constants
	ARRAY_LENGTH	= 10	; Number of elements in array
	STRING_LENGTH	= 11	; Length of string buffer (32 bit unsigned integer plus null character)
	DWORD_SIZE		= 4		; Number of bytes in DWORD
	MAX_DIGIT		= 57	; Maximum character code for digits
	MIN_DIGIT		= 48	; Minimum character code for digits
	NUM_SPACES		= 3		; Number of spaces to print between numbers in list
	
; Macros
	;Macro writes contents of string to the console and advances specified number of lines.
	;receives: string address, number of lines to advance.
	;preconditions: string is null terminated. numLines must be non-negative integer.
	;registers changed: N/A
	displayString MACRO stringOffset, numLines
		LOCAL	newLineLoop, exitMacro
		; push registers used
		push	edx
		push	ecx

		; set loop counter
		mov		ecx, numLines		

		; move and write string
		mov		edx, stringOffset	
		call	WriteString
		
		; If number of lines to advance is 0, jump to end.
		cmp		ecx, 0
		je		exitMacro

		newLineLoop: ; Call specified number of advances to new line.
			call	CrLf
			loop	newLineLoop

		exitMacro: ; Restore old register values and exit.
			pop		ecx
			pop		edx
	ENDM

	;Macro prompts the user for input, and stores input in a string buffer.
	;receives: prompt string address, address of string buffer for storing user input
	;preconditions: prompt is null terminated and string buffer size > 1 element.
	;registers changed: N/A
	getString MACRO promptOffset, stringOffset
		; Push registers used.
		push			ecx
		push			edx

		; Display prompt for data entry.
		displayString	promptOffset, 0

		; Read user entry.
		mov				edx, stringOffset
		mov				ecx, STRING_LENGTH
		call			ReadString

		; Restore old register values and exit.
		pop				edx
		pop				ecx
	ENDM

	;Macro prints number of spaces equal to argument received.
	;receives: integer
	;preconditions: argument is non-negative.
	;registers changed: N/A
	printSpace MACRO numSpace
		LOCAL printLoop
		; push registers used.
		push	ecx
		push	eax

		; Set loop counter and move space char into AL.
		mov		ecx, numSpace
		mov		al, ' '

		; Print desired number of spaces.
		printLoop:
			call	WriteChar
			loop	printLoop

		; Restore old register values and exit.
		pop		eax
		pop		ecx
	ENDM

.data
; Strings
	; Introduces the program and author.
	intro	BYTE	" Low-level I/O Procedures: by Joseph Sullivan", 13, 10, \
						"**********************************************", 13, 10, \
						"This program prompts the user to enter 10 integers.", 13, 10, \
						"Then, the integers, their sum, and their average are displayed.", 0

	; Prompt for user to enter data.
	prompt		BYTE	"Please enter a non-negative integer in the range [0 ... 4294967295]: ", 0

	; Buffer holds string of char digits entered by user.
	digitString	BYTE	STRING_LENGTH	DUP(?)	
	tempBuffer	BYTE	STRING_LENGTH	DUP(?)	

	; Error message displays if sum cannot be stored properly due to size.
	tooBigErr	BYTE	"The sum of these numbers is too large to hold in a 32 bit register.", 13, 10, \
						"As a result, I am unable to display the sum or the average of the these numbers.", 0

	invalid		BYTE	"Entry was invalid.", 0					; displays when input is invalid.
	arrayTitle	BYTE	"The numbers that you entered are:", 0	; Title for displaying integer array.
	sumTitle	BYTE	"The sum of these numbers is: ", 0		; Title for sum of integer array.
	aveTitle	BYTE	"The average of these numbers is: ", 0	; Title for average of integer array.
	goodbye		BYTE	"Goodbye.", 0							; Indicates that program has ended.
	
; Integers
	intArray	DWORD	ARRAY_LENGTH	DUP(?)	; array that stores converted integers.
	convertInt	DWORD	?						; Holds integer converted from string input.

.code
main PROC
	displayString	OFFSET intro, 2	;Display introduction of program and author.
	
	; Get 10 integers as strings input by user.
	push	OFFSET intArray		 
	push	OFFSET prompt		
	push	OFFSET digitString	
	push	OFFSET convertInt	
	push	OFFSET invalid		
	call	getData

	; Convert integers back to strings and display them.
	push	OFFSET arrayTitle	
	push	OFFSET intArray		
	push	OFFSET tempBuffer	
	push	OFFSET digitString	
	call	displayList

	; Calculate the sum and average of the integers, 
	; convert them to strings, and display them.
	push	OFFSET	intArray
	push	OFFSET	sumTitle
	push	OFFSET	tempBuffer
	push	OFFSET	digitString
	push	OFFSET	aveTitle
	push	OFFSET	tooBigErr
	call	displayStats
	
	displayString	OFFSET goodbye, 2	; Display message indicating end of program.

	exit	; exit to operating system
main ENDP

; Procedure sets up loop for reading input from user and places converted integer into array.
; receives: system stack: @intArray, @prompt, @digitString, @convertInt, @invalid.
; returns: intArray filled with integers.
; preconditions: N/A
; registers changed: N/A
getData PROC
	; Set up stack and save registers
	push	ebp
	mov		ebp, esp
	push	esi
	push	eax
	push	ebx
	push	ecx

	mov		esi, [ebp+24]		; intArray
	mov		ecx, ARRAY_LENGTH	; Set loop counter to number of integers to get.

	readValLoop: ; Read integer input as string from user until array is filled.
		push	[ebp+16] ; digitString 
		push	[ebp+20] ; prompt      
		push	[ebp+12] ; convertInt  
		push	[ebp+8]  ; invalid    
		call	readVal

		; Move converted integer into array of 32 bit elements.
		mov		eax, [ebp+12] ; convertInt
		mov		ebx, [eax]
		mov		[esi], ebx

		; Move to next empty array element and get next entry.
		add		esi, DWORD_SIZE
		loop	readValLoop
	
	; Cleanup stack, restore registers, and return.
	call	CrLf
	pop		ecx
	pop		ebx
	pop		eax
	pop		esi
	pop		ebp
	ret		20
getData	ENDP

; Procedure gets integer entered as string, validates entry, and converts from string to integer.
; receives: system stack: @digitString, @prompt, @convertInt, @invalid.
; returns: convertInt containing integer converted from string.
; preconditions: N/A
; registers changed: N/A
readVal PROC
	; Set up stack and save registers.
	push	ebp
	mov		ebp, esp
	push	esi
	push	eax
	push	ebx
	push	ecx
	push	edx
	
	cld		; Clear direction flag for loading string bytes.

	getInput: ; Set esi to beginning of array and get string input of an integer from user.
		mov			esi, [ebp+20]		; digitString
		getString	[ebp+16], esi		; prompts user for integer and reads input as string.

		; If first character in string buffer is NULL, input is invalid.
		mov		eax, 0
		mov		al, [esi]
		cmp		eax, 0
		je		error
		
		; Clear contents of convertInt for use in loop.
		mov		ebx, [ebp+12] 	
		mov		edx, 0				
		mov		[ebx], edx		
			
	conversionLoop: ; Validate entry and convert string to integer.
		; Move next string byte into AL register.
		mov		eax, 0
		lodsb

		; If byte equals 0, NULL character has ended string.
		cmp		eax, 0				
		je		stringEnd

		; bytes hold values between 48 and 57, else input is invalid.
		cmp		eax, MIN_DIGIT
		jl		error
		cmp		eax, MAX_DIGIT
		jg		error

		sub		eax, MIN_DIGIT	; Subtract minimum char digit code to get actual integer digit.
		mov		ecx, eax		; temporarily store digit in ecx

		; Multiply convertInt by 10 to move all digits up one position.
		mov		eax, [ebx] ; convertInt
		mov		edx, 10	
		mul		edx

		; move adjusted integer back into convertInt and add next digit stored in ecx.
		mov		[ebx], eax 
		add		[ebx], ecx

		; If carry flag was set by addition, number is too large for 32-bit destination.
		mov		eax, 0
		adc		eax, 0
		cmp		eax, 1
		je		error

		jmp		conversionLoop ; continue, converting next string character to digit.

	error: ; Display error message and ask user for new entry.
	displayString [ebp+8], 1
	jmp getInput

	stringEnd: ; Clean up stack, restore registers, and return.
		pop		edx
		pop		ecx
		pop		ebx
		pop		eax
		pop		esi
		pop		ebp
		ret		16
readVal ENDP


; Procedure displays the list of array values.
; receives: system stack: @ arrayTitle, @ intArray, @tempBuffer, @digitString
; returns: N/A
; preconditions: N/A
; registers changed: N/A
displayList PROC
	; Set up stack and save registers.
	push	ebp
	mov		ebp, esp
	push	esi
	push	ecx

	displayString	[ebp+20], 1 ; Print arrayTitle.

	mov		esi, [ebp+16]		; intArray
	mov		ecx, ARRAY_LENGTH	; Set loop counter.

	arrayPrintNext: ; print value of next array element.
		push		[esi]				; value in current intArray index.
		push		[ebp+12]			; tempBuffer
		push		[ebp+8]				; digitString
		call		writeVal			; convert integer to string and print it.

		printSpace	NUM_SPACES			; Print 3 spaces after each number.

		; Move to next array element to print and call writeVal again.
		add			esi, DWORD_SIZE		
		loop		arrayPrintNext		;
		call		CrLf
		call		CrLf

	; Cleanu up stack, restore registers, and return.
	pop		ecx
	pop		esi
	pop		ebp
	ret		16
displayList	ENDP

; Procedure Converts integer to string and displays it.
; receives: system stack: integer value, @tempBuffer, @digitString.
; returns: N/A
; preconditions: value is non-negative 32-bit integer.
; registers changed: N/A
writeVal PROC
	; Set up stack and local variables.
	LOCAL	intHolder:DWORD	; Holds value while eax is used for calculations.
	push	edi
	push	esi
	push	eax
	push	ebx
	push	ecx
	push	edx

	cld		; Clear direction flag for loading string.
	mov		eax, [ebp+16]	; Move 32 bit integer into eax.
	mov		edi, [ebp+12]	; tempBuffer
	mov		ecx, 0			; clear ecx for incrementation of loop counter

	charConvert: ; Convert value from integer to string.
		; Divide by 10 to get lowest digit of integer as remainder in edx.
		mov		ebx, 10
		mov		edx, 0
		div		ebx

		mov		intHolder, eax	; Hold rest of int while eax is used in other operations.
		add		edx, MIN_DIGIT	; Add 48 to remainder to get char code.

		; Load char code into temp string buffer.
		mov		eax, edx		
		stosb

		inc		ecx				; increment loop counter for later use.
		mov		eax, intHolder	; return whats left of int to eax.

		; If next digit is 0, Last digit has been converted.
		cmp		eax, 0
		jne		charConvert


		mov		esi, edi		; Move last buffer position incremented to by stosb into esi
		dec		esi				; decrement to move to byte last filled by stosb
		mov		edi, [ebp+8]	; digitString

		; String stored at esi needs to be reversed. Loop below starts at last position in esi
		; and places it at the beginning of the string buffer in edi, then esi moves to
		; previous position, and edi advances to next position to reverse the string.
		reverseString:
		std		; decrement mode
		lodsb	; load char into AL
		cld		; increment mode
		stosb	; store char in digitString
		loop	reverseString

		; Add null character to end of string.
		mov		eax, 0
		mov		[edi], eax

		; Display the integer that is now converted to a string.
		displayString [ebp+8], 0
	
	; Clean up stack, restore register, and return.
	pop		edx
	pop		ecx
	pop		ebx
	pop		eax
	pop		esi
	pop		edi
	ret		12
writeVal ENDP

; Procedure calculates and displays the sum and average of an integer array.
; receives: system stack: @intArray, @sumTitle, @tempBuffer, @digitString, @aveTitle, @tooBigErr.
; returns: N/A
; preconditions: N/A
; registers changed: N/A
displayStats	PROC
	; Set up stack.
	push	ebp
	mov		ebp, esp
	push	eax
	push	ebx
	push	ecx
	push	edx
	push	esi
		
	mov		ecx, ARRAY_LENGTH	; Set loop counter.
	mov		eax, 0				; Clear eax before summing integers in it.
	mov		esi, [ebp+28]		;intArray
	
	addLoop: ; Sum integers from array in eax
		add		eax, [esi]

		; If carry flag is set sum is too large for 32 bit register.
		mov		ebx, 0
		adc		ebx, 0
		cmp		ebx, 1
		je		tooLarge

		; ; Move esi to next array element and call addLoop again.
		add		esi, DWORD_SIZE 
		loop	addLoop

		; display sum
		displayString [ebp+24], 0	; Display sum title
		push	eax					; sum
		push	[ebp+20]			; tempBuffer
		push	[ebp+16]			; digitString.
		call	WriteVal			; Convert sum to string and display.
		call	CrLf
		call	CrLf

		; calculate average (sum / array length)
		mov		ebx, ARRAY_LENGTH
		mov		edx, 0
		div		ebx

		; display average
		displayString [ebp+12], 0	; Display ave title
		push	eax					; average
		push	[ebp+20]			; tempBuffer
		push	[ebp+16]			; digitString
		call	WriteVal			; Convert average to string and display.
		jmp		exitProc			; jump to end of procedure.

		tooLarge:	; Displays an error message if sum is too large for 32 bit register.
			displayString [ebp+8], 0

		exitProc:	; Clean up stack, restore registers, and return.
		call	CrLf
		call	CrLf
		pop		esi
		pop		edx
		pop		ecx
		pop		ebx
		pop		eax
		pop		ebp	
		ret		24
displayStats	ENDP

END	main