IncludeFile "FastPixel.pbi"

#WIDTH = 1024
#HEIGHT = 768

Global Dim pixelArray.l(#WIDTH * #HEIGHT)
Global.f angle, centerX, centerY, angleSpeed, zoomSpeed, zoom

Procedure DrawRotozoom()
	Protected o, x, y, u, v, color
	Protected cosAngle.f = Cos(angle), sinAngle.f = Sin(angle)
	Protected t.f = ElapsedMilliseconds() * 0.001
	Protected c = (1 + Sin(t * 2)) * 127
	c = RGBA(c, 255-c, 32,32)

	For py = 0 To #HEIGHT - 1
		For px = 0 To #WIDTH - 1
			x = (px - centerX) * zoom
			y = (py - centerY) * zoom
			u = (x * cosAngle - y * sinAngle) * 0.01
			v = (x * sinAngle + y * cosAngle) * 0.01
			If (u + v) & 1
				pixelArray(o) = RGBA(255,255,255, 255)
			ElseIf (u * v) & 25 = 0
				pixelArray(o) = c
			EndIf
			o + 1
		Next
	Next
	
	angle + angleSpeed
	angleSpeed = Sin(t * 0.5) * 0.005
	zoom = 2 - Sin(t * 0.5) * 0.5
	centerX = #WIDTH * 0.5 + Sin(angle) * zoom * 350
	centerY = #HEIGHT * 0.5 + Cos(angle) * zoom * 350
EndProcedure

If OpenWindow(0, 0, 0, #WIDTH, #HEIGHT, "Rotozoom Checkerboard")
	If FPStart(WindowID(0), 0, 0, #WIDTH, #HEIGHT, @pixelArray())
		Repeat
			FPClear(RGBA(0, 0, 0, 32))
 			DrawRotozoom()
			FPDraw()
			
			fps + 1
			If ElapsedMilliseconds() > fpt
				SetWindowTitle(0, "Rotozoom Checkerboard - FPS: " + Str(fps))
				fps = 0 : fpt = ElapsedMilliseconds() + 1000
			EndIf
 			While WindowEvent() : If Event() = #PB_Event_CloseWindow: Break 2 : EndIf : Wend
		ForEver
		FPEnd()
	EndIf
EndIf
; IDE Options = PureBasic 6.11 LTS Beta 3 (Windows - x64)
; CursorPosition = 33
; Folding = -
; EnableXP
; Executable = FastPixel.exe
; DisableDebugger