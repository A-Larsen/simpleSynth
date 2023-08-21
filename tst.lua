local audio = require("audio")

audio.masterAmp(.5)

audio.loadOsillator("sine", 50, 0.1)
audio.loadWav("seaShells.wav")

audio.playWav()
audio.playOscillator()
audio.play()

while true do
    local s = io.read(1)
    print(s)
end
