cc=g++ --static
exe=FlappyBird
obj=bird.o win_manager.o game.o

$(exe):$(obj)
	$(cc) -o $(exe) $(obj)
bird.o:bird.cc
	$(cc) -c bird.cc
win_manager.o:win_manager.cc
	$(cc) -c win_manager.cc
game.o:game.cc
	$(cc) -c game.cc