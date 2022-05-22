import cv2
import math
import numpy as np

'''
    This script takes 2 sudoku's generated from the cpp script, and makes images out of them
'''

pathToSolved = 'sudokuPuzzle_solved_9x9.txt'
pathToUnsolved = 'sudokuPuzzle_unsolved_9x9.txt'
PixelwidthImg = 1000

# pathToSolved = 'sudokuPuzzle_solved_100x100.txt'
# pathToUnsolved = 'sudokuPuzzle_unsolved_100x100.txt'
# PixelwidthImg = 3000


def read_sudoku(pathToTxtFile):
    with open(pathToTxtFile) as f:
        lines = f.readlines()
    f.close()

    sudoku = [int(val) for val in lines]

    return sudoku


def show_sudoku(sudoku_array, widthImg):
    widthAndHeight = int(math.sqrt(len(sudoku_array)))

    img_show = np.ones((widthImg, widthImg, 3))
    w_per_box = widthImg/widthAndHeight
    boxw = int(math.sqrt(widthAndHeight))
    w_per_smallbox = widthImg/boxw

    kn = 0
    for i in range(widthAndHeight):
        for j in range(widthAndHeight):
            currentValue = sudoku_array[kn]
            if currentValue != -1:

                # Show the image
                img_show = cv2.rectangle(img_show, (int((i)*w_per_box), int((j)*w_per_box)),
                              (int((i+1.0)*w_per_box), int((j+1.0)*w_per_box)),
                              ((currentValue/widthAndHeight, currentValue/widthAndHeight, 255)), -1)
                # center the text
                textsize = cv2.getTextSize(str(currentValue), cv2.FONT_HERSHEY_SIMPLEX, 0.45, 1)[0]
                cv2.putText(img_show, str(currentValue), (int((i+0.5)*w_per_box-textsize[0]/2),
                                                          int((j+0.5)*w_per_box+textsize[1]/2)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.45, (0,0,0))
            else:
                # a black box, showing which rectangles we create
                img_show = cv2.rectangle(img_show, (int(i * w_per_box), int(j * w_per_box)),
                                         (int((i + 1.0) * w_per_box), int((j + 1.0) * w_per_box)),
                                         (0, 0, 0), -1)

            cv2.rectangle(img_show, (int(i * w_per_box), int( j * w_per_box)),
                          (int((i+1.0)*w_per_box), int((j+1.0)*w_per_box)), (0, 0, 0), 3)
            kn += 1
    # draw the small boxes
    for i in range(boxw):
        for j in range(boxw):
            cv2.rectangle(img_show, (int(i * w_per_smallbox), int(j * w_per_smallbox)),
                          (int((i + 1.0) * w_per_smallbox), int((j + 1.0) * w_per_smallbox)), (0, 0, 0), 7)

    # big box
    cv2.rectangle(img_show, (3, 3), (widthImg - 3, widthImg - 3), (0, 0, 0), 10)
    return img_show


# read the sudokus
sudokuSolved = read_sudoku(pathToSolved)
sudokuUnsolved = read_sudoku(pathToUnsolved)

# show the sudokus
imgSolved = show_sudoku(sudokuSolved, PixelwidthImg)
imgUnsolved = show_sudoku(sudokuUnsolved, PixelwidthImg)
imgTotal = np.concatenate([imgUnsolved, imgSolved], axis=1)

# write the sudoku to a file
cv2.imwrite("VisualisationSudoku.jpg", imgTotal*255)

# show the image we just created
cv2.imshow("Check", imgTotal)
cv2.waitKey(-1)
