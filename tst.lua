local audio = require("audio")

audio.masterAmp(.3)

local osc_1 = audio.loadOsillator("square", 40, 0.5)
local osc_2 = audio.loadOsillator("square", 60, 0.5)
local wav_1 = audio.loadWav("seaShells.wav")
local wav_2 = audio.loadWav("i_win.wav")

audio.playWav(wav_1)
audio.playWav(wav_2)
audio.playOscillator(osc_1)
audio.playOscillator(osc_2)
audio.play()

while true do
    local s = io.read(1)
    print(s)
end
