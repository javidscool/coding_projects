*-----------------------------------------------------------
* Program    :
* Written by :
* Date       :
* Description:
*-----------------------------------------------------------
START   ORG     $1000
RED     equ     $000000FF
GREEN   equ     $0000FF00
BLUE    equ     $00FF0000
YELLOW  equ     RED+GREEN
ORANGE  equ     $003090FF
BLACK   equ     $00000000
WHITE   equ     $00FFFFFF

ALL_REG                 REG     D0-D7/A0-A6

BALL_COLOR                 equ     ORANGE
BALL_WIDTH                 equ     50
BALL_HEIGHT                equ     50
HALF_BALL                  equ     BALL_HEIGHT/2

VERT_PADDLE_HEIGHT      equ     150  
VERT_PADDLE_WIDTH       equ     16
HALF_VERT_HEIGHT        equ     VERT_PADDLE_HEIGHT/2
HALF_VERT_WIDTH         equ     VERT_PADDLE_WIDTH/2

HORIZ_PADDLE_HEIGHT     equ     16
HORIZ_PADDLE_WIDTH      equ     150
HALF_HORIZ_WIDTH        equ     HORIZ_PADDLE_WIDTH/2
HALF_HORIZ_HEIGHT       equ     HORIZ_PADDLE_HEIGHT/2

;possible keyboard input
SPACE_INPUT     equ     $20
KEY_Q_INPUT     equ     $51
KEY_W_INPUT     equ     $57
KEY_T_INPUT     equ     $54
KEY_G_INPUT     equ     $47
KEY_N_INPUT     equ     $4E
KEY_M_INPUT     equ     $4D
KEY_O_INPUT     equ     $4F
KEY_L_INPUT     equ     $4C

RECTANGLE_UPPER_X            equ     15
RECTANGLE_UPPER_Y            equ     15
RECTANGLE_LOWER_X            equ     785
RECTANGLE_LOWER_Y            equ     585

UPPER_X_COLL                 equ     RECTANGLE_UPPER_X+35
UPPER_Y_COLL                 equ     RECTANGLE_UPPER_Y+35
LOWER_X_COLL                 equ     RECTANGLE_LOWER_X-35
LOWER_Y_COLL                 equ     RECTANGLE_LOWER_Y-35

LEFT_PADDLE_COLL        equ     RECTANGLE_UPPER_X+45
RIGHT_PADDLE_COLL       equ     RECTANGLE_LOWER_X-45
TOP_PADDLE_COLL         equ     RECTANGLE_UPPER_Y+45
BOTTOM_PADDLE_COLL      equ     RECTANGLE_LOWER_Y-45

LEFT_PLACE        equ     LEFT_PADDLE_COLL+HALF_BALL
RIGHT_PLACE       equ     RIGHT_PADDLE_COLL-HALF_BALL
TOP_PLACE         equ     TOP_PADDLE_COLL+HALF_BALL
BOTTOM_PLACE      equ     BOTTOM_PADDLE_COLL-HALF_BALL

RECTANGLE_COLOR              equ     RED

CLEAR_SCREEN_COMMAND            equ     11
GET_USER_INPUT_COMMAND          equ     19
SET_PEN_COLOR                   equ     80
SET_FILL_COLOR                  equ     81
DRAW_PIXEL                      equ     82  
DRAW_LINE_COMMAND               equ     84
DRAW_FILLED_RECTANGLE           equ     87
DRAW_BALL_COMMAND               equ     88
DRAW_RECTANGLE_COMMAND          equ     90
SET_DRAWING_MODE                equ     92
REPAINT                         equ     94

SCREEN_WIDTH                    equ     800
SCREEN_HEIGHT                   equ     600
MID_SCREEN_WIDTH                equ     SCREEN_WIDTH/2
MID_SCREEN_HEIGHT               equ     SCREEN_HEIGHT/2 


DIGIT_WIDTH     equ     30
DIGIT_HEIGHT    equ     30
DIGIT_COLOR     equ     WHITE

;top paddle score position
TOP_SCORE_X        equ     MID_SCREEN_WIDTH     
TOP_SCORE_Y        equ     RECTANGLE_UPPER_Y+100

;left paddle score position
LEFT_SCORE_X        equ     RECTANGLE_UPPER_X+100     
LEFT_SCORE_Y        equ     MID_SCREEN_HEIGHT

;bottom paddle score position
BOTTOM_SCORE_X        equ     MID_SCREEN_WIDTH     
BOTTOM_SCORE_Y        equ     RECTANGLE_LOWER_Y-100

;right paddle score position
RIGHT_SCORE_X        equ     RECTANGLE_LOWER_X-100     
RIGHT_SCORE_Y        equ     MID_SCREEN_HEIGHT

END_SCORE               equ     20

        ;setting screen resolution
        move.b #33,D0
        move.l #SCREEN_WIDTH*$10000+SCREEN_HEIGHT,d1        ;set screen to 800 x 600
        TRAP   #15

        ;clear screen
        move.b  #CLEAR_SCREEN_COMMAND,d0
        move.l  #$FF00,d1
        TRAP    #15
        
        ;set-up double buffering
        move.l  #17,d1
        move.b  #SET_DRAWING_MODE,d0
        TRAP #15
        
        lea     Image,a0
        
        
keyPressWait
        move.l  #SPACE_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noStartInput
        
        move.b  NO_INPUT_YET,d0
        beq     arNoStInput
        move.b  #0,NO_INPUT_YET
        
        bra     drawLoop
        
noStartInput
        move.b  #1,NO_INPUT_YET
        
arNoStInput
        bra     keyPressWait
        

        
drawLoop

        ;setting the pen color for the pixel
        move.l  (a0),d1       ;moving the pixel color into d1
        move.b  #SET_PEN_COLOR,d0
        TRAP #15
        
        ;draw pixel
        move.w  X_POS,d1
        move.w  Y_POS,d2
        move.b  #DRAW_PIXEL,d0
        TRAP #15
        
        addi    #1, X_POS
        cmp     #SCREEN_WIDTH, X_POS
        beq     updatePos
        
        adda    #4,a0
        bra drawLoop
        
        
updatePos

        move.w  #0, X_POS
        subi    #1, Y_POS
        cmp     #0, Y_POS
        beq     begin
        
        adda    #4,a0
        bra drawLoop
        
        
begin
        addi    #35,LEFT_PADDLE_X 
        subi    #35,RIGHT_PADDLE_X
        addi    #35,TOP_PADDLE_Y
        subi    #35,BOTTOM_PADDLE_Y
        
        ;first we get time
        move.w  #8,d0
        TRAP    #15
        move.l  d1,BALL_PAST_TIME


         
        ;setting pen color for drawing the rectangle
        move.l  #RECTANGLE_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting rectangle coordinates
        move.w  #RECTANGLE_UPPER_X,d1
        move.w  #RECTANGLE_UPPER_Y,d2
        move.w  #RECTANGLE_LOWER_X,d3
        move.w  #RECTANGLE_LOWER_Y,d4
        
        ;drawing the rectangle
        move.b  #DRAW_RECTANGLE_COMMAND,d0
        TRAP #15
        
        ;this draws the background image and rectangle
        move.b  #REPAINT,d0
        TRAP #15
        
       
gameloop

        jsr updateBall
        
        jsr updateScores
        
        jsr updatePaddles
        
        move.l  TOP_SCORE,d1
        cmp     #END_SCORE,d1
        bge     resetGame
        
        move.l  BOTTOM_SCORE,d1
        cmp     #END_SCORE,d1
        bge     resetGame
        
        move.l  LEFT_SCORE,d1
        cmp     #END_SCORE,d1
        bge     resetGame
        
        move.l  RIGHT_SCORE,d1
        cmp     #END_SCORE,d1
        bge     resetGame
        
        bra gameLoop
        
        bra end
        
updateBall

        jsr     drawBall
        
        jsr     upBallPos
        
        jsr     XcollisionDetection
        
        rts
        
updatePaddles

        ;drawing the paddles
        jsr drawLeftPaddle
        
        jsr drawRightPaddle
        
        jsr drawTopPaddle
        
        jsr drawBottomPaddle
        
        
        ;checking for movement on the paddles
        jsr getTopQInput
        
        jsr getTopWInput
        
        jsr getBottNInput
        
        jsr getBottMInput
        
        jsr getLeftTInput
        
        jsr getLeftGInput
        
        jsr getRightOInput
        
        jsr getRightLInput
        
        rts
        
updateScores
        
        jsr drawTopScore
        
        jsr drawLeftScore
        
        jsr drawBottScore
        
        jsr drawRightScore
        
        rts
 
upBallPos

        ;updating ball position
        
        ;first we get time
        move.w  #8,d0
        TRAP    #15
        
        move.l  d1,BALL_FIN_TIME
        move.l  BALL_PAST_TIME,d2
        sub.l   d2,d1   ;getting delta t
        
        
        move.l  BALL_VEL_X, d0  ;moving the ball's x velocity into d0
        muls    d1,d0   ;this gives vel * time
        divs    #100,d0
        ext.l   d0       
        add.l   d0, BALL_X
        
        move.l  BALL_VEL_Y, d0  ;moving the ball's y velocity into d0
        muls    d1,d0   ;this gives vel * time
        divs    #100,d0
        ext.l   d0
        add.l   d0, BALL_Y
        
        
        move.l  BALL_FIN_TIME,d3
        move.l  d3,BALL_PAST_TIME
        move.l  BALL_PAST_TIME,d3
        
        clr.l   d0
        clr.l   d1
        clr.l   d2
        clr.l   d3
        
        rts
        
resetGame
        move.l  #0,TOP_SCORE
        move.l  #0,BOTTOM_SCORE
        move.l  #0,LEFT_SCORE
        move.l  #0,RIGHT_SCORE
        
        jsr drawInvTop
        jsr drawInvBott
        jsr drawInvLeft
        jsr drawInvRight
        
        move.l  #GREEN,TOP_COLOR
        move.l  #GREEN,LEFT_COLOR
        move.l  #GREEN,BOTTOM_COLOR
        move.l  #GREEN,RIGHT_COLOR
        
        bra gameLoop
        
***********************drawing the top score using drawSevenSegment******************************
;drawing the top score using drawSevenSegment
drawTopScore
        clr.l   d1
        move.l  TOP_SCORE,d1
        
        move.w  #0,d2
        cmp     d1,d2
        beq     drawTopZeroes      ;we draw two zeroes

        ;if the score isn't zero
        DIVS    #10,d1
        move.l  d1,d2
        swap    d2
        ext.l   d2
        ext.l   d1
        ;now d2 contains the right number and d1 contains the left number
        move.w  d2,RIGHT_DIGIT
        move.w  d1,LEFT_DIGIT
        
contDrawTop
        ;put info on stack so we can draw the left number
        move.w  LEFT_DIGIT,-(sp)
        move.w  #TOP_SCORE_X-20,-(sp)
        move.w  #TOP_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment 
        
        ;put info on stack so we can draw the right number
        move.w  RIGHT_DIGIT,-(sp)
        move.w  #TOP_SCORE_X+20,-(sp)
        move.w  #TOP_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment
        
        move.w  #TOP_SCORE_X-20,d0
        move.w  #TOP_SCORE_Y,d1
        move.w  #TOP_SCORE_X+20,d2
        move.w  #TOP_SCORE_Y,d3


        
        move.w  (sp)+,LEFT_DIGIT
        move.w  (sp)+,d0
        move.w  (sp)+,d1 
        move.w  (sp)+,RIGHT_DIGIT
        move.w  (sp)+,d2
        move.w  (sp)+,d3 
        
        rts

drawTopZeroes
        move.w  #0,RIGHT_DIGIT
        move.w  #0,LEFT_DIGIT
        bra contDrawTop
***********************end drawing the top score using drawSevenSegment**************************
        


***********************drawing the left score using drawSevenSegment*****************************        
;drawing the left score using drawSevenSegment
drawLeftScore
        clr.l   d1
        move.l  LEFT_SCORE,d1
        
        move.w  #0,d2
        cmp     d1,d2
        beq     drawLeftZeroes      ;we draw two zeroes

        ;if the score isn't zero
        DIVS    #10,d1
        move.l  d1,d2
        swap    d2
        ext.w   d2
        ext.w   d1
        ;now d2 contains the right number and d1 contains the left number
        move.w  d2,RIGHT_DIGIT
        move.w  d1,LEFT_DIGIT
        
contDrawLeft
        ;put info on stack so we can draw the left number
        move.w  LEFT_DIGIT,-(sp)
        move.w  #LEFT_SCORE_X-20,-(sp)
        move.w  #LEFT_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment 
        
        ;put info on stack so we can draw the right number
        move.w  RIGHT_DIGIT,-(sp)
        move.w  #LEFT_SCORE_X+20,-(sp)
        move.w  #LEFT_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment
        
        move.w  #LEFT_SCORE_X-20,d0
        move.w  #LEFT_SCORE_Y,d1
        move.w  #LEFT_SCORE_X+20,d2
        move.w  #LEFT_SCORE_Y,d3


        
        move.w  (sp)+,LEFT_DIGIT
        move.w  (sp)+,d0
        move.w  (sp)+,d1 
        move.w  (sp)+,RIGHT_DIGIT
        move.w  (sp)+,d2
        move.w  (sp)+,d3 
        
        rts
        
drawLeftZeroes
        move.w  #0,RIGHT_DIGIT
        move.w  #0,LEFT_DIGIT
        bra contDrawLeft
***********************end drawing the left score using drawSevenSegment*************************



***********************drawing the bott score using drawSevenSegment*****************************         
;drawing the bottom score using drawSevenSegment
drawBottScore
        move.l  BOTTOM_SCORE,d1
        
        move.w  #0,d2
        cmp     d1,d2
        beq     drawBottZeroes      ;we draw two zeroes

        ;if the score isn't zero
        DIVS    #10,d1
        move.l  d1,d2
        swap    d2
        ext.w   d2
        ext.w   d1
        ;now d2 contains the right number and d1 contains the left number
        move.w  d2,RIGHT_DIGIT
        move.w  d1,LEFT_DIGIT

contDrawBott        
        ;put info on stack so we can draw the left number
        move.w  LEFT_DIGIT,-(sp)
        move.w  #BOTTOM_SCORE_X-20,-(sp)
        move.w  #BOTTOM_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment 
        
        ;put info on stack so we can draw the right number
        move.w  RIGHT_DIGIT,-(sp)
        move.w  #BOTTOM_SCORE_X+20,-(sp)
        move.w  #BOTTOM_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment
        
        move.w  #BOTTOM_SCORE_X-20,d0
        move.w  #BOTTOM_SCORE_Y,d1
        move.w  #BOTTOM_SCORE_X+20,d2
        move.w  #BOTTOM_SCORE_Y,d3


        
        move.w  (sp)+,LEFT_DIGIT
        move.w  (sp)+,d0
        move.w  (sp)+,d1 
        move.w  (sp)+,RIGHT_DIGIT
        move.w  (sp)+,d2
        move.w  (sp)+,d3 
        
        rts

drawBottZeroes
        move.w  #0,RIGHT_DIGIT
        move.w  #0,LEFT_DIGIT
        bra contDrawBott
***********************end drawing the bott score using drawSevenSegment*************************
        
        
        
***********************drawing the right score using drawSevenSegment****************************
;drawing the right score using drawSevenSegment
drawRightScore
        clr.l   d1
        move.l  RIGHT_SCORE,d1
        
        move.w  #0,d2
        cmp     d1,d2
        beq     drawRightZeroes      ;we draw two zeroes

        ;if the score isn't zero
        DIVS    #10,d1
        move.l  d1,d2
        swap    d2
        ext.w   d2
        ext.w   d1
        ;now d2 contains the right number and d1 contains the left number
        move.w  d2,RIGHT_DIGIT
        move.w  d1,LEFT_DIGIT

contDrawRight        
        ;put info on stack so we can draw the left number
        move.w  LEFT_DIGIT,-(sp)
        move.w  #RIGHT_SCORE_X-20,-(sp)
        move.w  #RIGHT_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment 
        
        ;put info on stack so we can draw the right number
        move.w  RIGHT_DIGIT,-(sp)
        move.w  #RIGHT_SCORE_X+20,-(sp)
        move.w  #RIGHT_SCORE_Y,-(sp)
        
        jsr     drawSevenSegment
        
        move.w  #RIGHT_SCORE_X-20,d0
        move.w  #RIGHT_SCORE_Y,d1
        move.w  #RIGHT_SCORE_X+20,d2
        move.w  #RIGHT_SCORE_Y,d3


        
        move.w  (sp)+,LEFT_DIGIT
        move.w  (sp)+,d0
        move.w  (sp)+,d1 
        move.w  (sp)+,RIGHT_DIGIT
        move.w  (sp)+,d2
        move.w  (sp)+,d3 
        
        rts
        
drawRightZeroes
        move.w  #0,RIGHT_DIGIT
        move.w  #0,LEFT_DIGIT
        bra contDrawRight
***********************end drawing the right score using drawSevenSegment************************

        
        
drawSevenSegment
        clr.l   d6
        clr.l   d7
        ;clr.l   d3

        move.w  4(sp),DIGIT_CENTER_Y        ;putting in y-coord of center of number
        move.w  DIGIT_CENTER_Y,d0
        move.w  6(sp),DIGIT_CENTER_X        ;putting in x-coord of center of number
        move.w  DIGIT_CENTER_X,d1
        move.w  8(sp),d6        ;putting in number
        mulu    #2,d6
        
        lea     DIGITS,a0
        move.w  (a0,d6),d7      ;putting the value from the digits table into d7
        lea     SEG_OFFSET,a1
        move.w  SEG_COUNTER,d6

checkBits        
        btst    d6,d7
        beq     nextBitTest
        
        ;if the bit in digits is set...
        ;setting digit line pen color
        move.l  #DIGIT_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        move.w  d6,d5
        mulu    #8,d5
        move.w  (a1,d5),d1
        add.w   DIGIT_CENTER_X,d1
        move.w  2(a1,d5),d2
        add.w   DIGIT_CENTER_Y,d2
        move.w  4(a1,d5),d3
        add.w   DIGIT_CENTER_X,d3
        move.w  6(a1,d5),d4
        add.w   DIGIT_CENTER_Y,d4
        move.b  #DRAW_LINE_COMMAND,d0
        ;TRAP    #15
        ;move.b  #REPAINT,d0
        TRAP #15
        
nextBitTest
        subi    #1,d6
        move.w  #-1,d0
        cmp     d6,d0
        beq   endDraw
        bra checkBits
        
endDraw 
        ;move.w  (sp)+, DIGIT_CENTER_Y
        ;move.w  (sp)+, DIGIT_CENTER_X
        ;move.w  (sp)+, d6       
        rts
        
     
;;;;;;;;;;;;;;;;;TOP PADDLE INPUT;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;GET Q INPUT;;;;;;;;;;;;;;;;;;;;;;;
getTopQInput

        move.w TOP_PADDLE_X,-(sp)        
        
        ;getting possible q press
        move.l  #KEY_Q_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noTopInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoTopInput
        move.b  #0,NO_INPUT_YET
        
   
moveTopLeft
        movem.w (sp),d2         ;putting in x-coord
        
        subi    #HALF_HORIZ_WIDTH,d2
        sub.w   PADDLE_MOVE_INCR,d2   
        cmp     #LEFT_PADDLE_COLL,d2
        bgt     changeTopXNeg
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeTopXNeg
        addi    #HALF_HORIZ_WIDTH,d2
        move.w  d2,TOP_PADDLE_X     
        move.w  (sp)+,d2
        
        move.w  TOP_PADDLE_X,d1
        move.w  TOP_PADDLE_Y,d2
        
        add.w   #HORIZ_PADDLE_WIDTH>>1,d1
        
        
        move.w  d2,d4
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #8,-(sp)
        move.w #HORIZ_PADDLE_HEIGHT,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET Q INPUT;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;GET W INPUT;;;;;;;;;;;;;;;;;;;;;;;
getTopWInput

        move.w TOP_PADDLE_X,-(sp)        
        
        ;getting possible w press
        move.l  #KEY_W_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noTopInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoTopInput
        move.b  #0,NO_INPUT_YET
        
   
moveTopRight
        movem.w (sp),d2         ;putting in x-coord
        
        addi    #HALF_HORIZ_WIDTH,d2
        add.w   PADDLE_MOVE_INCR,d2   
        cmp     #RIGHT_PADDLE_COLL,d2
        blt     changeTopXPos
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeTopXPos
        subi    #HALF_HORIZ_WIDTH,d2
        move.w  d2,TOP_PADDLE_X     
        move.w  (sp)+,d2
        
        move.w  TOP_PADDLE_X,d1
        move.w  TOP_PADDLE_Y,d2
        
        sub.w   #HORIZ_PADDLE_WIDTH>>1,d1
        sub.w   PADDLE_MOVE_INCR,d1
        
        
        move.w  d2,d4
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #8,-(sp)
        move.w #HORIZ_PADDLE_HEIGHT,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET W INPUT;;;;;;;;;;;;;;;;;;;;;;;

noTopInput
        move.b  #1,NO_INPUT_YET
        
aroundNoTopInput

        move.w  (sp)+,TOP_PADDLE_X
        rts    
;;;;;;;;;;;;;;;;;END TOP PADDLE INPUT;;;;;;;;;;;;;;;;;;    



;;;;;;;;;;;;;;;;;BOTTOM PADDLE INPUT;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;GET N INPUT;;;;;;;;;;;;;;;;;;;;;;;
getBottNInput

        move.w BOTTOM_PADDLE_X,-(sp)        
        
        ;getting possible n press
        move.l  #KEY_N_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noBottInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoBottInput
        move.b  #0,NO_INPUT_YET
        
   
moveBottLeft
        movem.w (sp),d2         ;putting in x-coord
        
        subi    #HALF_HORIZ_WIDTH,d2
        sub.w   PADDLE_MOVE_INCR,d2   
        cmp     #LEFT_PADDLE_COLL,d2
        bgt     changeBottXNeg
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeBottXNeg
        addi    #HALF_HORIZ_WIDTH,d2
        move.w  d2,BOTTOM_PADDLE_X     
        move.w  (sp)+,d2
        
        move.w  BOTTOM_PADDLE_X,d1
        move.w  BOTTOM_PADDLE_Y,d2
        
        add.w   #HORIZ_PADDLE_WIDTH>>1,d1
        
        
        move.w  d2,d4
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #8,-(sp)
        move.w #HORIZ_PADDLE_HEIGHT,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET N INPUT;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;GET M INPUT;;;;;;;;;;;;;;;;;;;;;;;
getBottMInput

        move.w BOTTOM_PADDLE_X,-(sp)        
        
        ;getting possible m press
        move.l  #KEY_M_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noBottInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoBottInput
        move.b  #0,NO_INPUT_YET
        
   
moveBottRight
        movem.w (sp),d2         ;putting in x-coord
        
        addi    #HALF_HORIZ_WIDTH,d2
        add.w   PADDLE_MOVE_INCR,d2   
        cmp     #RIGHT_PADDLE_COLL,d2
        blt     changeBottXPos
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeBottXPos
        subi    #HALF_HORIZ_WIDTH,d2
        move.w  d2,BOTTOM_PADDLE_X     
        move.w  (sp)+,d2
        
        move.w  BOTTOM_PADDLE_X,d1
        move.w  BOTTOM_PADDLE_Y,d2
        
        sub.w   #HORIZ_PADDLE_WIDTH>>1,d1
        sub.w   PADDLE_MOVE_INCR,d1
        
        
        move.w  d2,d4
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #8,-(sp)
        move.w #HORIZ_PADDLE_HEIGHT,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET M INPUT;;;;;;;;;;;;;;;;;;;;;;;

noBottInput
        move.b  #1,NO_INPUT_YET
        
aroundNoBottInput

        move.w  (sp)+,BOTTOM_PADDLE_X
        rts    
;;;;;;;;;;;;;;;;;END BOTTOM PADDLE INPUT;;;;;;;;;;;;;;;;;;  



;;;;;;;;;;;;;;;;;LEFT PADDLE INPUT;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;GET T INPUT;;;;;;;;;;;;;;;;;;;;;;;
getLeftTInput

        move.w LEFT_PADDLE_Y,-(sp)        
        
        ;getting possible t press
        move.l  #KEY_T_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noLeftInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoLeftInput
        move.b  #0,NO_INPUT_YET
        
   
moveLeftUp
        movem.w (sp),d2         ;putting in y-coord
        
        subi    #HALF_VERT_HEIGHT,d2
        sub.w   PADDLE_MOVE_INCR,d2   
        cmp     #TOP_PADDLE_COLL,d2
        bgt     changeLeftYNeg
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeLeftYNeg
        addi    #HALF_VERT_HEIGHT,d2
        move.w  d2,LEFT_PADDLE_Y     
        move.w  (sp)+,d2
        
        move.w  LEFT_PADDLE_X,d1
        move.w  LEFT_PADDLE_Y,d2
        
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        
        
        move.w  d2,d4
        add.w   #VERT_PADDLE_HEIGHT>>1,d4
        add.w   PADDLE_MOVE_INCR,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #VERT_PADDLE_WIDTH,-(sp)
        move.w #8,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET T INPUT;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;GET G INPUT;;;;;;;;;;;;;;;;;;;;;;;
getLeftGInput

        move.w LEFT_PADDLE_Y,-(sp)        
        
        ;getting possible g press
        move.l  #KEY_G_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noLeftInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoLeftInput
        move.b  #0,NO_INPUT_YET
        
   
moveLeftDown
        movem.w (sp),d2         ;putting in y-coord
        
        addi    #HALF_VERT_HEIGHT,d2
        add.w   PADDLE_MOVE_INCR,d2   
        cmp     #BOTTOM_PADDLE_COLL,d2
        blt     changeLeftYPos
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeLeftYPos
        subi    #HALF_VERT_HEIGHT,d2
        move.w  d2,LEFT_PADDLE_Y     
        move.w  (sp)+,d2
        
        move.w  LEFT_PADDLE_X,d1
        move.w  LEFT_PADDLE_Y,d2
        
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        
        move.w  d2,d4
        sub.w   #VERT_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #VERT_PADDLE_WIDTH,-(sp)
        move.w #8,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET G INPUT;;;;;;;;;;;;;;;;;;;;;;;

noLeftInput
        move.b  #1,NO_INPUT_YET
        
aroundNoLeftInput

        move.w  (sp)+,LEFT_PADDLE_Y
        rts    
;;;;;;;;;;;;;;;;;END LEFT PADDLE INPUT;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;RIGHT PADDLE INPUT;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;GET O INPUT;;;;;;;;;;;;;;;;;;;;;;;
getRightOInput

        move.w RIGHT_PADDLE_Y,-(sp)        
        
        ;getting possible o press
        move.l  #KEY_O_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noRightInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoRightInput
        move.b  #0,NO_INPUT_YET
        
   
moveRightUp
        movem.w (sp),d2         ;putting in y-coord
        
        subi    #HALF_VERT_HEIGHT,d2
        sub.w   PADDLE_MOVE_INCR,d2   
        cmp     #TOP_PADDLE_COLL,d2
        bgt     changeRightYNeg
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeRightYNeg
        addi    #HALF_VERT_HEIGHT,d2
        move.w  d2,RIGHT_PADDLE_Y     
        move.w  (sp)+,d2
        
        move.w  RIGHT_PADDLE_X,d1
        move.w  RIGHT_PADDLE_Y,d2
        
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        
        
        move.w  d2,d4
        add.w   #VERT_PADDLE_HEIGHT>>1,d4
        add.w   PADDLE_MOVE_INCR,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #VERT_PADDLE_WIDTH,-(sp)
        move.w #8,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET O INPUT;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;GET L INPUT;;;;;;;;;;;;;;;;;;;;;;;
getRightLInput

        move.w RIGHT_PADDLE_Y,-(sp)        
        
        ;getting possible l press
        move.l  #KEY_L_INPUT,d1
        move.b  #GET_USER_INPUT_COMMAND,d0
        TRAP    #15
        andi.l  #$FF,d1
        beq     noRightInput
        
        move.b  NO_INPUT_YET,d0
        beq     aroundNoRightInput
        move.b  #0,NO_INPUT_YET
        
   
moveRightDown
        movem.w (sp),d2         ;putting in y-coord
        
        addi    #HALF_VERT_HEIGHT,d2
        add.w   PADDLE_MOVE_INCR,d2   
        cmp     #BOTTOM_PADDLE_COLL,d2
        blt     changeRightYPos
        
        ;if we don't branch
        move.w  (sp)+,d2
        rts
        
changeRightYPos
        subi    #HALF_VERT_HEIGHT,d2
        move.w  d2,RIGHT_PADDLE_Y     
        move.w  (sp)+,d2
        
        move.w  RIGHT_PADDLE_X,d1
        move.w  RIGHT_PADDLE_Y,d2
        
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        
        move.w  d2,d4
        sub.w   #VERT_PADDLE_HEIGHT>>1,d4

        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #VERT_PADDLE_WIDTH,-(sp)
        move.w #8,-(sp)
        
        bra drawInvalidSpace
        

;;;;;;;;;;;;;;;;;END GET G INPUT;;;;;;;;;;;;;;;;;;;;;;;

noRightInput
        move.b  #1,NO_INPUT_YET
        
aroundNoRightInput

        move.w  (sp)+,RIGHT_PADDLE_Y
        rts    
;;;;;;;;;;;;;;;;;END LEFT PADDLE INPUT;;;;;;;;;;;;;;;;;;
       

********************COLLISION DETECTION**************************************
        
        
XcollisionDetection
        
        ;collision detection on left/right side
        
        ;first check if collision on left side
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left side
        cmp     #UPPER_X_COLL,d1
        ble     resetLeftBall
        
        ;if no branch on that, check collision on right side
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right side
        cmp     #LOWER_X_COLL,d1
        bge     resetRightBall
        
        
        
        ;collision detection on left/right paddle
        
        ;first check if ball hits left paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1        
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left side of ball
        move.w  LEFT_PADDLE_X,d2        ;putting in x-coord of paddle
        addi    #HALF_VERT_WIDTH,d2     ;gives me right of paddle      
        cmp     d2,d1
        ble     checkLeftPaddleColl
        
        
        
        ;then check if ball hits right paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right side of ball
        move.w  RIGHT_PADDLE_X,d2       ;putting in x-coord of paddle
        subi    #HALF_VERT_WIDTH,d2     ;gives me left of paddle
        cmp     d2,d1
        bge     checkRightPaddleColl
               
        
YcollisionDetection
        
        ;collision detection on top/bottom
        
        ;check collision on upper side
        move.l  BALL_Y,d2
        divs    #16,d2
        ext.l   d2
        sub.w   #BALL_HEIGHT>>1,d2      ;gives me upper side
        cmp     #UPPER_Y_COLL,d2
        ble     resetTopBall
        
        ;check collision on bottom side
        move.l  BALL_Y,d2
        divs    #16,d2
        ext.l   d2
        add.w   #BALL_HEIGHT>>1,d2      ;gives me lower side
        cmp     #LOWER_Y_COLL,d2
        bge     resetBottBall
        
        ;collision detection on top/bottom paddle
        
        ;first check if ball hits top paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_HEIGHT>>1,d1      ;gives me top side of ball
        move.w  TOP_PADDLE_Y,d2         ;putting in y-coord of paddle
        addi    #HALF_HORIZ_HEIGHT,d2   ;gives me bottom of paddle
        cmp     d2,d1
        ble     checkTopPaddleColl
        
        ;then check if ball hits bottom paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_HEIGHT>>1,d1      ;gives me bottom side of ball
        move.w  BOTTOM_PADDLE_Y,d2      ;putting in y-coord of paddle
        subi    #HALF_HORIZ_HEIGHT,d2   ;gives me top of paddle
        cmp     d2,d1
        bge     checkBottomPaddleColl
        
        rts
        

;;;;;;;;;;;;;;;;;COLLISION DETECTION FOR LEFT PADDLE;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkLeftPaddleColl

        
 
        ;first check if top of ball is greater than top of paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_HEIGHT>>1,d1      ;gives me top of ball
        move.w  LEFT_PADDLE_Y,d2        ;putting in y-coord of paddle
        subi    #HALF_VERT_HEIGHT,d2    ;gives me top of paddle
        cmp     d2,d1                   ;checking if top of ball is greater than top of paddle
        bge     keepCheckingLeft
        
        ;next check if bottom of ball is greater than top of paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_HEIGHT>>1,d1      ;gives me bottom of ball
        move.w  LEFT_PADDLE_Y,d2        ;putting in y-coord of paddle
        subi    #HALF_VERT_HEIGHT,d2    ;gives me top of paddle
        cmp     d2,d1                   ;checking if bottom of ball is greater than top of paddle
        bge     keepCheckingLeft
        
        
        ;if we don't branch
        bra YcollisionDetection

keepCheckingLeft        
                
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_HEIGHT>>1,d1      ;gives me bottom of ball
        move.w  LEFT_PADDLE_Y,d2        ;putting in y-coord of paddle
        addi    #HALF_VERT_HEIGHT,d2    ;gives me bottom of paddle
        cmp     d2,d1                   ;checking if bottom of paddle is greater than bottom of ball
        ble     setLeftLast
        
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_HEIGHT>>1,d1      ;gives me top of ball
        move.w  LEFT_PADDLE_Y,d2        ;putting in y-coord of paddle
        addi    #HALF_VERT_HEIGHT,d2    ;gives me bottom of paddle
        cmp     d2,d1                   ;checking if bottom of paddle is greater than bottom of ball
        ble     setLeftLast
        
        ;if we don't branch
        bra YcollisionDetection

setLeftLast
        move.w  #1,LAST_TOUCHED
        move.l  #LEFT_PLACE<<4,BALL_X  
        ;move.l  BALL_X,d4
        bra     changeXVel
;;;;;;;;;;;;;;;;;END COLLISION DETECTION FOR LEFT PADDLE;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;COLLISION DETECTION FOR RIGHT PADDLE;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkRightPaddleColl
        
        ;first check if top of ball is greater than top of paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_HEIGHT>>1,d1      ;gives me top of ball
        move.w  RIGHT_PADDLE_Y,d2       ;putting in y-coord of paddle
        subi    #HALF_VERT_HEIGHT,d2    ;gives me top of paddle
        cmp     d2,d1                   ;checking if top of ball is greater than top of paddle
        bge     keepCheckingRight
        
        ;next check if bottom of ball is greater than top of paddle
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_HEIGHT>>1,d1      ;gives me bottom of ball
        move.w  RIGHT_PADDLE_Y,d2       ;putting in y-coord of paddle
        subi    #HALF_VERT_HEIGHT,d2    ;gives me top of paddle
        cmp     d2,d1                   ;checking if bottom of ball is greater than top of paddle
        bge     keepCheckingRight
        
        
        ;if we don't branch
        bra YcollisionDetection

keepCheckingRight

        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_HEIGHT>>1,d1      ;gives me bottom of ball
        move.w  RIGHT_PADDLE_Y,d2       ;putting in y-coord of paddle
        addi    #HALF_VERT_HEIGHT,d2    ;gives me bottom of paddle
        cmp     d2,d1                   ;checking if bottom of paddle is greater than bottom of ball
        ble     setRightLast
        
        move.l  BALL_Y,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_HEIGHT>>1,d1      ;gives me top of ball
        move.w  RIGHT_PADDLE_Y,d2       ;putting in y-coord of paddle
        addi    #HALF_VERT_HEIGHT,d2    ;gives me bottom of paddle
        cmp     d2,d1                   ;checking if bottom of paddle is greater than top of ball
        ble     setRightLast
        
        ;if we don't branch
        bra YcollisionDetection
        
setRightLast
        move.w  #3,LAST_TOUCHED
        move.l  #RIGHT_PLACE<<4,BALL_X  
        ;move.l  BALL_X,d4
        bra     changeXVel
;;;;;;;;;;;;;;;;;END COLLISION DETECTION FOR RIGHT PADDLE;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;COLLISION DETECTION FOR TOP PADDLE;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkTopPaddleColl
        
        ;first check if left of ball is greater than left of paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left of ball
        move.w  TOP_PADDLE_X,d2         ;putting in x-coord of paddle
        subi    #HALF_HORIZ_WIDTH,d2    ;gives me left of paddle
        cmp     d2,d1                   ;checking if left of ball is greater than left of paddle
        bge     keepCheckingTop
        
        ;next check if right of ball is greater than left of paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right of ball
        move.w  TOP_PADDLE_X,d2         ;putting in x-coord of paddle
        subi    #HALF_HORIZ_WIDTH,d2    ;gives me left of paddle
        cmp     d2,d1                   ;checking if right of ball is greater than left of paddle
        bge     keepCheckingTop
        
        
        ;if we don't branch
        rts

keepCheckingTop

        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right of ball
        move.w  TOP_PADDLE_X,d2         ;putting in x-coord of paddle
        addi    #HALF_HORIZ_WIDTH,d2    ;gives me right of paddle
        cmp     d2,d1                   ;checking if right of paddle is greater than right of ball
        ble     setTopLast
        
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left of ball
        move.w  TOP_PADDLE_X,d2         ;putting in x-coord of paddle
        addi    #HALF_HORIZ_WIDTH,d2    ;gives me right of paddle
        cmp     d2,d1                   ;checking if right of paddle is greater than left of ball
        ble     setTopLast
        
        ;if we don't branch
        rts
        
setTopLast
        move.w  #0,LAST_TOUCHED
        move.l  #TOP_PLACE<<4,BALL_Y  
        ;move.l  BALL_Y,d4
        bra     changeYVel
;;;;;;;;;;;;;;;;;END COLLISION DETECTION FOR TOP PADDLE;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;COLLISION DETECTION FOR BOTTOM PADDLE;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkBottomPaddleColl
        
        ;first check if left of ball is greater than left of paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left of ball
        move.w  BOTTOM_PADDLE_X,d2      ;putting in x-coord of paddle
        subi    #HALF_HORIZ_WIDTH,d2    ;gives me left of paddle
        cmp     d2,d1                   ;checking if left of ball is greater than left of paddle
        bge     keepCheckingBottom
        
        ;next check if right of ball is greater than left of paddle
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right of ball
        move.w  BOTTOM_PADDLE_X,d2      ;putting in x-coord of paddle
        subi    #HALF_HORIZ_WIDTH,d2    ;gives me left of paddle
        cmp     d2,d1                   ;checking if right of ball is greater than left of paddle
        bge     keepCheckingBottom
        
        
        ;if we don't branch
        rts

keepCheckingBottom

        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        add.w   #BALL_WIDTH>>1,d1       ;gives me right of ball
        move.w  BOTTOM_PADDLE_X,d2      ;putting in x-coord of paddle
        addi    #HALF_HORIZ_WIDTH,d2    ;gives me right of paddle
        cmp     d2,d1                   ;checking if right of paddle is greater than right of ball
        ble     setBottLast
        
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        sub.w   #BALL_WIDTH>>1,d1       ;gives me left of ball
        move.w  BOTTOM_PADDLE_X,d2      ;putting in x-coord of paddle
        addi    #HALF_VERT_HEIGHT,d2    ;gives me right of paddle
        cmp     d2,d1                   ;checking if bottom of paddle is greater than bottom of ball
        ble     setBottLast
        
        ;if we don't branch
        rts
        
setBottLast
        move.w  #2,LAST_TOUCHED
        move.l  #BOTTOM_PLACE<<4,BALL_Y  
        ;move.l  BALL_Y,d4
        bra     changeYVel
;;;;;;;;;;;;;;;;;END COLLISION DETECTION FOR BOTTOM PADDLE;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;RESET BALL IF IT HITS THE SIDE;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;Reset Left Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
resetLeftBall
       
        ;set up ball so that it comes back to middle of screen
        move.l  #MID_SCREEN_WIDTH<<4,d0 
        move.l  d0,BALL_X
        move.l  #MID_SCREEN_HEIGHT<<4,d0 
        move.l  d0,BALL_Y      
        
        ;set up ball velocity 
        move.l  BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        move.l  d0,BALL_VEL_X
        
        move.l  BALL_VEL_Y,d0 
        sub.l   BALL_VEL_Y,d0
        sub.l   BALL_VEL_Y,d0
        move.l  d0,BALL_VEL_Y
        
        clr.l   d0
        ;dispense scores
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
        
        move.w  LAST_TOUCHED, d0
        cmp     #0,d0
        bne     chkLeftBott
        
        ;if we don't branch, add an extra point to the left player
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        
        
chkLeftBott        
        move.w  LAST_TOUCHED, d0
        cmp     #2,d0
        bne     chkLeftRight
        
        ;if we don't branch, add an extra point to the bottom player
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        

chkLeftRight        
        move.w  LAST_TOUCHED, d0
        cmp     #3,d0
        bne     drawLeftInv
        
        ;if we don't branch, add an extra point to the right player
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
drawLeftInv
        jsr     drawInvTop
        
        jsr     drawInvBott
        
        jsr     drawInvRight
        
        move.w  #-1,LAST_TOUCHED
        
        jsr     setColor
        
        rts
;;;;;;;;;;;;;;;;;;;;;;;;;;End Reset Left Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;Reset Right Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
resetRightBall
        
        ;set up ball so that it comes back to middle of screen
        move.l  #MID_SCREEN_WIDTH<<4,d0 
        move.l  d0,BALL_X
        move.l  #MID_SCREEN_HEIGHT<<4,d0 
        move.l  d0,BALL_Y      
        
        ;set up ball velocity 
        move.l  BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        move.l  d0,BALL_VEL_X
        
        move.l  BALL_VEL_Y,d0 
        sub.l   BALL_VEL_Y,d0
        sub.l   BALL_VEL_Y,d0
        move.l  d0,BALL_VEL_Y
        
        clr.l   d0
        ;dispense scores
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        
        
        move.w  LAST_TOUCHED, d0
        cmp     #0,d0
        bne     chkRightBott
        
        ;if we don't branch, add an extra point to the left player
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        
        
chkRightBott        
        move.w  LAST_TOUCHED, d0
        cmp     #2,d0
        bne     chkRightLeft
        
        ;if we don't branch, add an extra point to the bottom player
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        

chkRightLeft        
        move.w  LAST_TOUCHED, d0
        cmp     #1,d0
        bne     drawRightInv
        
        ;if we don't branch, add an extra point to the right player
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        
drawRightInv   
        jsr     drawInvTop
        
        jsr     drawInvBott
        
        jsr     drawInvLeft
        
        move.w  #-1,LAST_TOUCHED
        
        jsr     setColor
        
        rts
;;;;;;;;;;;;;;;;;;;;;;;;;;End Reset Right Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        

;;;;;;;;;;;;;;;;;;;;;;;;;;Reset Top Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
resetTopBall
        
        ;set up ball so that it comes back to middle of screen
        move.l  #MID_SCREEN_WIDTH<<4,d0 
        move.l  d0,BALL_X
        move.l  #MID_SCREEN_HEIGHT<<4,d0 
        move.l  d0,BALL_Y      
        
        ;set up ball velocity 
        move.l  BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        move.l  d0,BALL_VEL_X
        
        move.l  BALL_VEL_Y,d0 
        sub.l   BALL_VEL_Y,d0
        sub.l   BALL_VEL_Y,d0
        move.l  d0,BALL_VEL_Y
        
        clr.l   d0
        ;dispense scores
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
        
        move.w  LAST_TOUCHED, d0
        cmp     #1,d0
        bne     chkTopBott
        
        ;if we don't branch, add an extra point to the left player
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        
        
chkTopBott        
        move.w  LAST_TOUCHED, d0
        cmp     #2,d0
        bne     chkTopRight
        
        ;if we don't branch, add an extra point to the bottom player
        move.w  #1,d0
        add.l   d0,BOTTOM_SCORE
        

chkTopRight        
        move.w  LAST_TOUCHED, d0
        cmp     #3,d0
        bne     drawTopInv
        
        ;if we don't branch, add an extra point to the right player
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
drawTopInv   
        jsr     drawInvLeft
        
        jsr     drawInvBott
        
        jsr     drawInvRight
        
        move.w  #-1,LAST_TOUCHED
        
        jsr     setColor
        
        rts
;;;;;;;;;;;;;;;;;;;;;;;;;;End Reset Top Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        
;;;;;;;;;;;;;;;;;;;;;;;;;;Reset Bottom Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
resetBottBall

        ;set up ball so that it comes back to middle of screen
        move.l  #MID_SCREEN_WIDTH<<4,d0 
        move.l  d0,BALL_X
        move.l  #MID_SCREEN_HEIGHT<<4,d0 
        move.l  d0,BALL_Y      
        
        clr.l   d0
        ;set up ball velocity 
        move.l  BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        move.l  d0,BALL_VEL_X
        
        move.l  BALL_VEL_Y,d0 
        sub.l   BALL_VEL_Y,d0
        sub.l   BALL_VEL_Y,d0
        move.l  d0,BALL_VEL_Y
        
        clr.l   d0
        ;dispense scores
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
        
        move.w  LAST_TOUCHED, d0
        cmp     #0,d0
        bne     chkBottLeft
        
        ;if we don't branch, add an extra point to the top player
        move.w  #1,d0
        add.l   d0,TOP_SCORE
        
        
chkBottLeft        
        move.w  LAST_TOUCHED, d0
        cmp     #1,d0
        bne     chkBottRight
        
        ;if we don't branch, add an extra point to the left player
        move.w  #1,d0
        add.l   d0,LEFT_SCORE
        

chkBottRight        
        move.w  LAST_TOUCHED, d0
        cmp     #3,d0
        bne     drawBottInv
        
        ;if we don't branch, add an extra point to the right player
        move.w  #1,d0
        add.l   d0,RIGHT_SCORE
        
drawBottInv
        jsr     drawInvTop
        
        jsr     drawInvLeft
        
        jsr     drawInvRight
        
        move.w  #-1,LAST_TOUCHED
        
        jsr     setColor
        
        rts   
;;;;;;;;;;;;;;;;;;;;;;;;;;End Reset Bottom Ball;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;Setting the colors of the paddles every time the scores change;;;;;;;;;;;; 
setColor
        lea     COLOR_TABLE,a0
        move.l  #0,d7
        move.l  #0,CUR_TOP
        
        move.l  #GREEN,TOP_COLOR
        move.l  #GREEN,LEFT_COLOR
        move.l  #GREEN,BOTTOM_COLOR
        move.l  #GREEN,RIGHT_COLOR
        
        
*****setting the top paddle color*************** 
        move.l  TOP_SCORE,d0
       
        move.l  LEFT_SCORE,d1
        cmp     d1,d0
        bge     colTopBott
        
        add.l   #4,d7

colTopBott       
        move.l  BOTTOM_SCORE,d1
        cmp     d1,d0
        bge     colTopRight
        
        add.l   #4,d7

colTopRight
        move.l  RIGHT_SCORE,d1
        cmp     d1,d0
        bge     endColTop
        
        add.l   #4,d7
        
endColTop       
        move.l  (a0,d7),TOP_COLOR
        move.l  TOP_COLOR,d0

*****end setting top paddle color***************       


*****setting the bottom paddle color************  
        move.l  #0,d7
        move.l  BOTTOM_SCORE,d0
      
        move.l  TOP_SCORE,d1
        cmp     d1,d0
        bge     colBottLeft
        
        add.l   #4,d7

colBottLeft       
        move.l  LEFT_SCORE,d1
        cmp     d1,d0
        bge     colBottRight
        
        add.l   #4,d7

colBottRight
        move.l  RIGHT_SCORE,d1
        cmp     d1,d0
        bge     endColBott
        
        add.l   #4,d7
        
endColBott       
        move.l  (a0,d7),BOTTOM_COLOR
        move.l  BOTTOM_COLOR,d0

*****end setting bottom paddle color***************        


*****setting the left paddle color************  
        move.l  #0,d7
        move.l  LEFT_SCORE,d0
        
        move.l  TOP_SCORE,d1
        cmp     d1,d0
        bge     colLeftBott
        
        add.l   #4,d7

colLeftBott       
        move.l  BOTTOM_SCORE,d1
        cmp     d1,d0
        bge     colLeftRight
        
        add.l   #4,d7

colLeftRight
        move.l  RIGHT_SCORE,d1
        cmp     d1,d0
        bge     endColLeft
        
        add.l   #4,d7
        
endColLeft       
        move.l  (a0,d7),LEFT_COLOR
        move.l  LEFT_COLOR,d0

*****end setting left paddle color*************** 


*****setting the right paddle color************  
        move.l  #0,d7
        move.l  RIGHT_SCORE,d0
        
        move.l  TOP_SCORE,d1
        cmp     d1,d0
        bge     colRightLeft
        
        add.l   #4,d7

colRightLeft       
        move.l  LEFT_SCORE,d1
        cmp     d1,d0
        bge     colRightBott
        
        add.l   #4,d7

colRightBott
        move.l  BOTTOM_SCORE,d1
        cmp     d1,d0
        bge     endColRight
        
        add.l   #4,d7
        
endColRight       
        move.l  (a0,d7),RIGHT_COLOR
        move.l  RIGHT_COLOR,d0

*****end setting right paddle color*************** 
        
        ;now all paddles should be set to proper colors
        rts
        
;;;;;;;;;;;end setting the colors of the paddles every time the scores change;;;;;;;;;;;; 
        
       
drawInvTop

        ;put info on stack so we can draw the invalid space
        move.w  #TOP_SCORE_X-20,d0
        sub     #DIGIT_WIDTH/2,d0
        move.w  #TOP_SCORE_Y,d1
        add     #DIGIT_HEIGHT,d1
        movem.w d0,-(sp)
        movem.w d1,-(sp)
        move.w #90,-(sp)
        move.w #DIGIT_HEIGHT*2,-(sp)
        
        bra drawInvalidSpace

drawInvBott

        ;put info on stack so we can draw the invalid space
        move.w  #BOTTOM_SCORE_X-20,d0
        sub     #DIGIT_WIDTH/2,d0
        move.w  #BOTTOM_SCORE_Y,d1
        add     #DIGIT_HEIGHT,d1
        movem.w d0,-(sp)
        movem.w d1,-(sp)
        move.w #90,-(sp)
        move.w #DIGIT_HEIGHT*2,-(sp)
        
        bra drawInvalidSpace
        
drawInvLeft

        ;put info on stack so we can draw the invalid space
        move.w  #LEFT_SCORE_X-20,d0
        sub     #DIGIT_WIDTH/2,d0
        move.w  #LEFT_SCORE_Y,d1
        add     #DIGIT_HEIGHT,d1
        movem.w d0,-(sp)
        movem.w d1,-(sp)
        move.w #90,-(sp)
        move.w #DIGIT_HEIGHT*2,-(sp)
        
        bra drawInvalidSpace
        
drawInvRight

        ;put info on stack so we can draw the invalid space
        move.w  #RIGHT_SCORE_X-20,d0
        sub     #DIGIT_WIDTH/2,d0
        move.w  #RIGHT_SCORE_Y,d1
        add     #DIGIT_HEIGHT,d1
        movem.w d0,-(sp)
        movem.w d1,-(sp)
        move.w #90,-(sp)
        move.w #DIGIT_HEIGHT*2,-(sp)
        
        bra drawInvalidSpace
        
        
;;;;;;;;;;;;;;;;;END RESET BALL IF IT HITS THE SIDE;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
        
        
        
changeXVel
        move.l  BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        sub.l   BALL_VEL_X,d0
        move.l  d0,BALL_VEL_X
        bra     YcollisionDetection
        
changeYVel

        move.l  BALL_VEL_Y,d0 
        sub.l   BALL_VEL_Y,d0
        sub.l   BALL_VEL_Y,d0
        move.l  d0,BALL_VEL_Y
        rts
        
********************END COLLISION DETECTION**************************************        

        
drawBall

        ;setting ball pen color
        move.l  #BALL_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting ball fill color
        move.l  #BALL_COLOR,d1
        move.b  #SET_FILL_COLOR,d0
        TRAP    #15
        
        
        move.l  BALL_X,d1
        divs    #16,d1
        ext.l   d1
        move.l  BALL_Y,d2
        divs    #16,d2
        ext.l   d2
        
        move.w  d1,d3
        sub.w   #BALL_WIDTH>>1,d1
        add.w   #BALL_WIDTH>>1,d3
        
        
        move.w  d2,d4
        sub.w   #BALL_HEIGHT>>1,d2
        add.w   #BALL_HEIGHT>>1,d4

        move.b  #DRAW_BALL_COMMAND,d0
        TRAP #15
        
        ;when I repaint, I draw the invalid portion from last loop and the valid portion from this loop AT THE SAME TIME (so shit looks decent)
        move.b  #REPAINT,d0
        TRAP #15
        
        ;put info on stack so we can draw the invalid space
        movem.w d1,-(sp)
        movem.w d4,-(sp)
        move.w #BALL_WIDTH,-(sp)
        move.w #BALL_HEIGHT,-(sp)
        
        bra drawInvalidSpace
        
        
drawLeftPaddle

        ;setting paddle pen color
        move.l  LEFT_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting paddle fill color
        move.l  LEFT_COLOR,d1
        move.b  #SET_FILL_COLOR,d0
        TRAP    #15
        
        
        move.w  LEFT_PADDLE_X,d1
        move.w  LEFT_PADDLE_Y,d2
        
        move.w  d1,d3
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        add.w   #VERT_PADDLE_WIDTH>>1,d3
        
        
        move.w  d2,d4
        sub.w   #VERT_PADDLE_HEIGHT>>1,d2
        add.w   #VERT_PADDLE_HEIGHT>>1,d4

        move.b  #DRAW_FILLED_RECTANGLE,d0
        TRAP #15
        
        rts
        
        
drawRightPaddle

        ;setting paddle pen color
        move.l  RIGHT_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting paddle fill color
        move.l  RIGHT_COLOR,d1
        move.b  #SET_FILL_COLOR,d0
        TRAP    #15
        
        
        move.w  RIGHT_PADDLE_X,d1
        move.w  RIGHT_PADDLE_Y,d2
        
        move.w  d1,d3
        sub.w   #VERT_PADDLE_WIDTH>>1,d1
        add.w   #VERT_PADDLE_WIDTH>>1,d3
        
        
        move.w  d2,d4
        sub.w   #VERT_PADDLE_HEIGHT>>1,d2
        add.w   #VERT_PADDLE_HEIGHT>>1,d4

        move.b  #DRAW_FILLED_RECTANGLE,d0
        TRAP #15
        
        rts
        
        
drawTopPaddle

        ;setting paddle pen color
        move.l  TOP_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting paddle fill color
        move.l  TOP_COLOR,d1
        move.b  #SET_FILL_COLOR,d0
        TRAP    #15
        
        
        move.w  TOP_PADDLE_X,d1
        move.w  TOP_PADDLE_Y,d2
        
        move.w  d1,d3
        sub.w   #HORIZ_PADDLE_WIDTH>>1,d1
        add.w   #HORIZ_PADDLE_WIDTH>>1,d3
        
        
        move.w  d2,d4
        sub.w   #HORIZ_PADDLE_HEIGHT>>1,d2
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        move.b  #DRAW_FILLED_RECTANGLE,d0
        TRAP #15
        
        rts
        
        
drawBottomPaddle

        ;setting paddle pen color
        move.l  BOTTOM_COLOR,d1
        move.b  #SET_PEN_COLOR,d0
        TRAP    #15
        
        ;setting paddle fill color
        move.l  BOTTOM_COLOR,d1
        move.b  #SET_FILL_COLOR,d0
        TRAP    #15
        
        
        move.w  BOTTOM_PADDLE_X,d1
        move.w  BOTTOM_PADDLE_Y,d2
        
        move.w  d1,d3
        sub.w   #HORIZ_PADDLE_WIDTH>>1,d1
        add.w   #HORIZ_PADDLE_WIDTH>>1,d3
        
        
        move.w  d2,d4
        sub.w   #HORIZ_PADDLE_HEIGHT>>1,d2
        add.w   #HORIZ_PADDLE_HEIGHT>>1,d4

        move.b  #DRAW_FILLED_RECTANGLE,d0
        TRAP #15
        
        rts
        

;this is a general function where I pass in an upper left x, and y coord. and a width and height and invalidate the space        
drawInvalidSpace
        
        movem.w (sp),d6         ;putting in height
        movem.w 2(sp),d5        ;putting in width
        movem.w 4(sp),d4        ;putting in y coord.
        movem.w 6(sp),d3        ;putting in x coord.
        
        add.w   d3,d5           ;add x coord. to width to get right of square
        move.w  d4,d7
        sub.w   d6,d7           ;sub y coord. to height to get top of square
        
        lea     Image,a0        ;reloading the beginning of the file into a0
        
        move.w  d4,INV_Y                ;moving the y-coordinate into INV_Y
        move.l  #SCREEN_HEIGHT,d1       ;moving SCREEN_HEIGHT into d1
        sub.w   INV_Y,d1                ;setting d1 to SCREEN_HEIGHT-y
        move.l  #SCREEN_WIDTH,d2        ;moving SCREEN_WIDTH into d2
        mulu    d1,d2                   ;making d2 (SCREEN_HEIGHT-y)*SCREEN_WIDTH
        move.l  d3,INV_X                ;moving the x-coord. into INV_X
        add.l   INV_X,d2                ;making d2 be (x + ( (SCREEN_HEIGHT-y) * SCREEN_WIDTH) )
        addi.l  #1,d2                   ;making d2 be ( (x + 1) + ( (SCREEN_HEIGHT-y) * SCREEN_WIDTH) )
        move.l  d2,d1
        add.l   d1,d2         
        add.l   d1,d2
        add.l   d1,d2
        
        adda.l  d2,a0                   ;setting the proper starting address to get the pixel out of
        
        bra drawInvLoop
        
        
 
        
drawInvLoop
 
        ;setting the pen color for the pixel
        move.l  (a0),d1       ;moving the pixel color into d1
        move.b  #SET_PEN_COLOR,d0
        TRAP #15
  
        ;draw pixel
        move.l  INV_X,d1
        move.w  INV_Y,d2
        move.b  #DRAW_PIXEL,d0
        TRAP #15
        
        ;move.b  #REPAINT,d0
        ;TRAP #15
        
        move.l  #1,d0
        add.l   d0, INV_X
        move.l  INV_X,d0
        cmp     d5, d0
        bgt     updateInvPos
        
        adda    #4,a0
        bra drawInvLoop
        
updateInvPos

        ;move.b  #REPAINT,d0
        ;TRAP #15
        
        move.l  d3, INV_X
        subi    #1, INV_Y
        move.w  INV_Y,d0
        cmp     d7, d0
        blt     endInv
        
        
        
        lea     Image,a0        ;reloading the beginning of the file into a0

        
        move.l  #SCREEN_HEIGHT,d1       ;moving SCREEN_HEIGHT into d1
        sub.w   INV_Y,d1                ;setting d1 to SCREEN_HEIGHT-y
        move.l  #SCREEN_WIDTH,d2        ;moving SCREEN_WIDTH into d2
        mulu    d1,d2                   ;making d2 (SCREEN_HEIGHT-y)*SCREEN_WIDTH
        add.l   INV_X,d2                ;making d2 be (x + ( (SCREEN_HEIGHT-y) * SCREEN_WIDTH) )
        addi.l    #1,d2                 ;making d2 be ( (x + 1) + ( (SCREEN_HEIGHT-y) * SCREEN_WIDTH) )
        move.l  d2,d1
        add.l   d1,d2         
        add.l   d1,d2
        add.l   d1,d2
        
        adda.l  d2,a0                   ;setting the proper starting address to get the pixel out of
        bra drawInvLoop

endInv

        movem.w (sp)+,d6
        movem.w (sp)+,d5
        move.w  (sp)+,d4
        move.w  (sp)+,d3
        
        rts

        
        

end
        MOVE.B  #9,D0
        TRAP    #15             Halt Simulator

;ball stuff
BALL_X  dc.l    MID_SCREEN_WIDTH<<4
BALL_Y  dc.l    MID_SCREEN_HEIGHT<<4
BALL_VEL_X      dc.l    300<<4
BALL_VEL_Y      dc.l    300<<4
BALL_PAST_TIME  dc.l    0<<4
BALL_FIN_TIME   dc.l    0<<4

COLOR_TABLE     dc.l    GREEN, BLUE, YELLOW, RED

;left paddle stuff
LEFT_PADDLE_X           dc.w    RECTANGLE_UPPER_X
LEFT_PADDLE_Y           dc.w    MID_SCREEN_HEIGHT
LEFT_COLOR              dc.l    GREEN

;right paddle stuff
RIGHT_PADDLE_X          dc.w    RECTANGLE_LOWER_X
RIGHT_PADDLE_Y          dc.w    MID_SCREEN_HEIGHT
RIGHT_COLOR             dc.l    GREEN

;top paddle stuff
TOP_PADDLE_X            dc.w    MID_SCREEN_WIDTH
TOP_PADDLE_Y            dc.w    RECTANGLE_UPPER_Y
TOP_COLOR               dc.l    GREEN

;bottom paddle stuff
BOTTOM_PADDLE_X         dc.w    MID_SCREEN_WIDTH
BOTTOM_PADDLE_Y         dc.w    RECTANGLE_LOWER_Y
BOTTOM_COLOR            dc.l    GREEN

PADDLE_MOVE_INCR        dc.w    8

COUNTER         dc.w    10

INV_X   dc.l    0
INV_Y   dc.w    0

Image      INCBIN  "cool.bin"
X_POS   dc.w    0
Y_POS   dc.w    SCREEN_HEIGHT

NO_INPUT_YET    ds.b    1

;seven segment display digits
DIGITS  dc.w    $7E, $12, $3D, $37, $53, $67, $4F, $32, $7F, $73

;segment offsets                
SEG_OFFSET      dc.w    -DIGIT_WIDTH/2, 0, DIGIT_WIDTH/2, 0                             ;start x,y and end x,y for seg g          
                dc.w    DIGIT_WIDTH/2, 0, DIGIT_WIDTH/2, DIGIT_HEIGHT                   ;start x,y and end x,y for seg f
                dc.w    -DIGIT_WIDTH/2, DIGIT_HEIGHT, DIGIT_WIDTH/2, DIGIT_HEIGHT       ;start x,y and end x,y for seg e
                dc.w    -DIGIT_WIDTH/2, 0, -DIGIT_WIDTH/2, DIGIT_HEIGHT                 ;start x,y and end x,y for seg d
                dc.w    DIGIT_WIDTH/2, -DIGIT_HEIGHT, DIGIT_WIDTH/2, 0                  ;start x,y and end x,y for seg c
                dc.w    -DIGIT_WIDTH/2, -DIGIT_HEIGHT, DIGIT_WIDTH/2, -DIGIT_HEIGHT     ;start x,y and end x,y for seg b
                dc.w    -DIGIT_WIDTH/2, -DIGIT_HEIGHT, -DIGIT_WIDTH/2, 0                ;start x,y and end x,y for seg a
                
SEG_COUNTER     dc.w    6

;constants for use in drawSevenSegment
DIGIT_CENTER_X  dc.w    0
DIGIT_CENTER_Y  dc.w    0

;left and right digits
LEFT_DIGIT      dc.l    0
RIGHT_DIGIT     dc.l    0

;player scores
TOP_SCORE       dc.l    0
BOTTOM_SCORE    dc.l    0
LEFT_SCORE      dc.l    0
RIGHT_SCORE     dc.l    0

LAST_TOUCHED    dc.w    -1

CUR_TOP         dc.l    0
        
        END     START
*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
