word_count: word_count.c
	gcc -Wall -o word_count `pkg-config --cflags glib-2.0` word_count.c `pkg-config --libs glib-2.0`
	./word_count pp.txt > pp_results.txt

ex_compile: ex_compile.c
	gcc -Wall -o ex_compile `pkg-config --cflags glib-2.0` ex_compile.c `pkg-config --libs glib-2.0`

hash: ex‑ghashtable‑1.c
	gcc -Wall -o ex‑ghashtable‑1 `pkg-config --cflags glib-2.0` ex‑ghashtable‑1.c `pkg-config --libs glib-2.0`

run:
	./ex_compile
	./ex‑ghashtable‑1

clean:
	rm word_count
