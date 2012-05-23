default : demo_helloworld demo_paint demo_3d demo_sound demo_netclient demo_netserver 

demo_helloworld : demo_helloworld.c
	gcc -o demo_helloworld -O3 demo_helloworld.c -lglut -lpolo -lportaudio

demo_paint : demo_paint.c
	gcc -o demo_paint -O3 demo_paint.c -lglut -lpolo -lportaudio

demo_3d : demo_3d.c
	gcc -o demo_3d -O3 demo_3d.c -lGLU -lglut -lpolo -lportaudio

demo_sound : demo_sound
	gcc -o demo_sound -O3 demo_sound.c -lglut -lpolo -lportaudio

demo_netclient : demo_netclient.c
	gcc -o demo_netclient -O3 demo_netclient.c -lglut -lpolo -lportaudio

demo_netserver : demo_netserver.c
	gcc -o demo_netserver -O3 demo_netserver.c -lglut -lpolo -lportaudio

