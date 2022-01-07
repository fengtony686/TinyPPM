import cv2 as cv
from cv2 import dnn_superres

img = cv.imread('./results/art_2077.png')
dst = cv.fastNlMeansDenoisingColored(img, None, 10, 10, 7, 21)
sr = dnn_superres.DnnSuperResImpl_create()
sr.readModel('./results/EDSR_x3.pb')
sr.setModel("edsr", 3)
result = sr.upsample(img)
cv.imwrite('./results/art_2077_superres.png', result)
cv.imwrite('./results/art_2077_antiNoising.png', dst)

