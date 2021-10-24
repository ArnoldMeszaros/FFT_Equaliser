def set_presets(preset):
    #the amplitudes of the gaussian functions

    if(preset == "bass"):
        amplitude_31 = 6
        amplitude_62 = 4
        amplitude_125 = 2
        amplitude_250 = 0
        amplitude_500 = 0
        amplitude_1000 = 0
        amplitude_2000 = 0
        amplitude_4000 = 0
        amplitude_8000 = 0
        amplitude_16000 = 0
    elif(preset == "middle"):
        amplitude_31 = 0
        amplitude_62 = 0
        amplitude_125 = 0
        amplitude_250 = 5
        amplitude_500 = 5
        amplitude_1000 = 5
        amplitude_2000 = 2
        amplitude_4000 = 0
        amplitude_8000 = 0
        amplitude_16000 = 0
    elif(preset == "treble"):
        amplitude_31 = 0
        amplitude_62 = 0
        amplitude_125 = 0
        amplitude_250 = 0
        amplitude_500 = 0
        amplitude_1000 = 0
        amplitude_2000 = 2
        amplitude_4000 = 5
        amplitude_8000 = 5
        amplitude_16000 = 7
    else:
        print("Presets can be: bass,middle,treble.")
        
    #an array which contains the amplitudes of the gaussian functions   
    amplitude =[amplitude_31,
    amplitude_62,
    amplitude_125,
    amplitude_250,
    amplitude_500,
    amplitude_1000,
    amplitude_2000,
    amplitude_4000,
    amplitude_8000,
    amplitude_16000]

    return amplitude
            