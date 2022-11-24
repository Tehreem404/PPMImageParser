def ppm_parser(fnum):
    fname = str(fnum) + ".ppm"
    f = open(fname, errors="ignore")

    whd = []
    p = ""
    line_count = 0
    for line in f:
        if len(whd) == 3:
            break
        line_count += 1
        for word in line.split():
            if word == "#":
                break
            if "P" in word:
                p = word
                continue
            if len(whd) < 3:
                whd.append(int(word))
    
    r = []
    g = []
    b = []
    count = 0
    if p == "P6":
        file = open(fname, 'rb')
        index = 0
        i = 0
        data = file.read()
        for byte in data:
            if i < line_count:
                if byte == 10:
                    i += 1
                continue
            if index % 3 == 0:
                count += 1
                r.append(byte)
            elif index % 3 == 1:
                count += 1
                g.append(byte)
            else:
                count += 1
                b.append(byte)
            index += 1
    else: 
        file = open(fname, 'r')
        i = 0
        index = 0
        for line in file:
            for word in line.split():
                if i < line_count:
                    i += 1
                    break
                if index % 3 == 0:
                    count += 1
                    r.append(int(word))
                elif index % 3 == 1:
                    count += 1
                    g.append(int(word))
                else:
                    count += 1
                    b.append(int(word))                
                index += 1
    rgb = []
    for i in range(0, int(count/3)):
        rgb.append(r[i])
        rgb.append(g[i])
        rgb.append(b[i])

    file.close()
    return(whd, r, g, b, rgb)