# luna-tamagotchi
my second project for fallout

table of contents:
- A short description of what my project is
- A couple sentences on why i made the project
- A couple sentences on how to use my project

**what the project is:**
- a custom tamagotchi consisting of a pcb designed in kiad and a case designed in the CAD software Fusion360
what the tamagotchi consists of:
- - a 3d case + lid
  - the pcb which consists of: XIAO-ESP32-C6, buzzer, rotary encoder, 4 buttons, and a 0.96" screen.

**why i chose to make with project (the inspiration):**
- as i am still new to hardware, i thought having a tutorial as a fallback for if i got stuck would be useful (avoid endless rabbit holes hhh). also i remember using a tamagotchi when i was younger and thought it was pretty cool
- so i decided to make a tamagotchi but to personalize it, so i made it of my friend luna.
- therefore as the firmware i planned to make it have many of the same interests/personality traits as my friend
- also the shape of a pig (an inside joke)

**how to use my project**
- first print off etc. the pcb and case. many different providers to do this.
- then to load the firmware (found in this respository), you can:
```
download the arduino IDE, install all the relevant libraries: Adafruit SSD1306 (by Adafruit), Adafruit GFX Library (by Adafruit). then
load up the xiao-esp32-c6 board. plug the project into your computer via usb, and press port on the arduino guide. as this project doesnt have a battery built
in, in order for this project to work it has to have a usb connected to it at all times.
for more help you can refer to these two guides:
https://fallout.hackclub.com/docs/guided-projects/tamagotchi-firmware
https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/
```

**how the project works**
- the project is mainly just a tamogatchi with a few little changes.
- the pet has different characteristics, hunger, happiness, energy, age, intelligence
- there are 4 main states: `nap [SJ]`, `feed luosifen [LSF]`, `climb a mountain [CLMB]`, `wildcard [WLDCRD]`.
- nap: energy increases by 15 points, intelligence increases by 10 points.
- feed: hunger increases by 10 points (means it is getting less hungry), energy increases by 5 points
- play: happiness increases by 10 points, energy decreases by 5 points
- wildcard: this is where the rotary encoder comes into play, you select a random orientation on the tamogotchi which corresponds to a number
0-19 inclusive. a random number is then selected, if you pick that number, the one below it or above it, the pets happiness is doubled! however if you
dont guess correctly, its decreased by 15%!! are you willing to risk your pets happiness?
- all values apart from age are capped in the ranges 1 to 100.
- the aim to get all values as high as possible.
- this is all done behind the scenes with the pcb, controlled by the XIAO-ESP32-C6.

here are some images of the project:

<img width="1385" height="754" alt="image" src="https://github.com/user-attachments/assets/7eeda688-ca41-4809-a17f-1069b659e289" />
<img width="902" height="683" alt="image" src="https://github.com/user-attachments/assets/446dcf23-8e21-4a45-8d2e-b9cfef02f151" />
<img width="965" height="508" alt="image" src="https://github.com/user-attachments/assets/b7a043b6-b535-4ef0-8cd6-394817818454" />
(there are a few components missing in this image because the STEP files for them didnt load, however shouldnt have any effect when it comes to
the actual PCB fabrication.)
<img width="775" height="728" alt="image" src="https://github.com/user-attachments/assets/28663ce6-32e1-4a9c-a8d2-e989c9f9ddcc" />
<img width="699" height="708" alt="image" src="https://github.com/user-attachments/assets/99fd1e33-5514-4094-ad77-a0cd235e113e" />


the BOM:
TODO!!! ACTUALLY WRITE THE BOM....

estimated costs (via uploading gerber files to jlcpcb):
<img width="1810" height="1091" alt="image" src="https://github.com/user-attachments/assets/748cb1b4-ed08-46ca-80c8-b35896231b79" />
[link](https://cart.jlcpcb.com/quote?stencilLayer=2&stencilWidth=100&stencilLength=100&stencilCounts=5&plateType=1&spm=Jlcpcb.Homepage.1010)




  
