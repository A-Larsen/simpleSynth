local audio = require("audio")
audio.playOscillator("square", 50, 0.5)
audio.loadWav()

while true do
    local s = io.read(1)
    print(s)
end
