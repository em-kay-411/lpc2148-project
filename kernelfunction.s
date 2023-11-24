		AREA    |.text|, CODE, READONLY
		EXPORT  my_function

my_function
		ADD       R0,PC,#0x00000020
		BL        0x0000024C
		ADD       R0,PC,#0x0000003C
		BL        0x0000024C
		B         0x000007F4
		BL        0x00000660
		AND       R4,R0,#0x000000FF
		MOV       R0,R4
		BL        0x00000688
		B         0x000007E4
		ENDIF
			
		END
			
			

