import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Create figure for plotting
fig, (ax, bx, cx) = plt.subplots(3, 1)


x_len = 500         # Number of points to display
y_range = [0, 50]   # Range of possible Y values to display

xs = list(range(0, x_len))
yRaw = [0] * x_len
yMA = [0] * x_len
yB = [0] * x_len

ax.set_ylim(y_range)
bx.set_ylim(y_range)
cx.set_ylim([-0.2, 1.2])

# Initialize COM port
ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1) 

lineRaw, = ax.plot(xs, yRaw)
lineMA, = bx.plot(xs, yMA)
lineMeasurement, = cx.plot(xs, yB)
line = [lineRaw, lineMA, lineMeasurement]



# This function is called periodically from FuncAnimation
def animate(i, y0, y1, y2):
    l = "0;0;0;"

    # Read from COM port
    try:
        l = ser.readline().decode("ascii").strip()
    except Exception:
        pass
    
    l = [t for t in l.split(";")[:-1]]


    # Add y to lists
    if len(l) >= 3:
        y0.append(int(l[0]))
        y1.append(int(l[1]))
        y2.append(int(l[2]))
        if len(l) == 4:
            print("Received character:", l[3])

    # Limit y lists to set number of items
    y0 = y0[-x_len:]
    y1 = y1[-x_len:]
    y2 = y2[-x_len:]

    # Update lines with new Y values
    line[0].set_ydata(y0)
    line[1].set_ydata(y1)
    line[2].set_ydata(y2)

    return line[0], line[1], line[2]


# Animation of the plots
ani = animation.FuncAnimation(fig,
    animate,
    fargs=(yRaw, yMA, yB),
    interval=1,
    blit=True)

# Add labels
# plt.title('Signal vizualization')
ax.set_title("Raw signal")
bx.set_title("Filtered signal (Moving average)")
cx.set_title("Binary value")
plt.xlabel('Samples')

plt.show()
