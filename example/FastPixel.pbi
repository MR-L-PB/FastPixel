Prototype FPInitProto(windowID, x.l, y.l, width.l, height.l, *pixelArray)
Prototype FPDrawProto()
Prototype FPClearProto(rgba.l)
Prototype FPFreeProto()

Global FPLib
Global FPInit.FPInitProto
Global FPDraw.FPDrawProto
Global FPClear.FPClearProto
Global FPFree.FPFreeProto

Procedure FPStart(windowID, x.l, y.l, width.l, height.l, *pixelArray)
	Global result
	Global FPLib = OpenLibrary(#PB_Any, "FastPixel.dll")
	If IsLibrary(FPLib)
		FPInit = GetFunction(FPLib, "FPInit")
		FPDraw = GetFunction(FPLib, "FPDraw")
		FPClear = GetFunction(FPLib, "FPClear")
		FPFree = GetFunction(FPLib, "FPFree")
		result = FPInit(windowID, x, y, width, height, *pixelArray)
	EndIf
	ProcedureReturn result
EndProcedure

Procedure FPEnd()
	If IsLibrary(FPLib)
		FPFree()
		CloseLibrary(FPLib)
	EndIf
	FPLib = 0
EndProcedure
; IDE Options = PureBasic 6.11 LTS Beta 3 (Windows - x64)
; CursorPosition = 26
; Folding = -
; Optimizer
; EnableXP
; Compiler = PureBasic 6.11 LTS Beta 3 - C Backend (Windows - x64)