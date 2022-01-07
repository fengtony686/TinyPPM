from PIL import Image, ImageDraw

imagePath = "./results/diamond_"

redImage = Image.open(imagePath + "red.jpg")
greenImage = Image.open(imagePath + "green.jpg")
blueImage = Image.open(imagePath + "blue.jpg")

w, h = redImage.size
redImage = redImage.convert('RGB')
greenImage = greenImage.convert('RGB')
blueImage = blueImage.convert('RGB')

colorArray = []
for x in range(w):
    for y in range(h):
        r, _, _ = redImage.getpixel((x, y))
        _, g, _ = greenImage.getpixel((x, y))
        _, _, b = blueImage.getpixel((x, y))
        rgb = (r, g, b)
        colorArray.append(rgb)

dipersionImage = Image.new('RGB', (w, h), (255, 255, 255))
draw = ImageDraw.Draw(dipersionImage)
i = 0
for x in range(w):
    for y in range(h):
        draw.point((x, y), fill=colorArray[i])
        i = i + 1
dipersionImage.save('./results/diamond.png', 'png')
