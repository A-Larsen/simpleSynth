local audio = require("audio")
audio.loadWav()
audio.playOscillator("square", 50, 0.5)
audio.play()

while true do
    local s = io.read(1)
    print(s)
end
