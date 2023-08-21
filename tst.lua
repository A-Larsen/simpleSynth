local audio = require("audio")

audio.playOscillator("sine", 50, 0.1)
audio.loadWav("seaShells.wav")
audio.play()

while true do
    local s = io.read(1)
    print(s)
end
