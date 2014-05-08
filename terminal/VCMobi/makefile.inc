mofcomp: mobiusb.bmf

mobiusb.bmf: mobiusb.mof
        mofcomp -B:mobiusb.bmf mobiusb.mof
        wmimofck mobiusb.bmf





