calculator: calculator.cpp
	g++ -o calculator calculator.cpp -lfltk -lfltk_images -lfltk_forms

clean:
	rm -f calculator

.PHONY: clean