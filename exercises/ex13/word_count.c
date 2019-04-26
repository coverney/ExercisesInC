/* Example code for Exercises in C

Copyright 2016 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

Started with ex-ghashtable-3.c from
http://www.ibm.com/developerworks/linux/tutorials/l-glib/section5.html

Note: this version leaks memory.

Further edited by Cassandra Overney

Run with:
make word_count
valgrind --leak-check=yes ./word_count

Results:
==7796== LEAK SUMMARY:
==7796==    definitely lost: 0 bytes in 0 blocks
==7796==    indirectly lost: 0 bytes in 0 blocks
==7796==      possibly lost: 0 bytes in 0 blocks
==7796==    still reachable: 18,604 bytes in 6 blocks
==7796==         suppressed: 0 bytes in 0 blocks
==7796== Reachable blocks (those to which a pointer was found) are not shown.
==7796== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==7796==
==7796== For counts of detected and suppressed errors, rerun with: -v
==7796== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

The "still reachable" blocks seem to be in the GLib hash table.

*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>

/* Represents a word-frequency pair. */
typedef struct {
    gint freq;
    gchar *word;
} Pair;

/* Compares two key-value pairs by frequency. */
gint compare_pair(gpointer v1, gpointer v2, gpointer user_data)
{
    Pair *p1 = (Pair *) v1;
    Pair *p2 = (Pair *) v2;
    return p1->freq - p2->freq;
}

/* Iterator that prints pairs. */
void pair_printor(gpointer value, gpointer user_data)
{
    Pair *pair = (Pair *) value;
    printf("%d\t %s\n", pair->freq, pair->word);
}

/* Iterator that frees pairs. */
void pair_free(gpointer value, gpointer user_data)
{
    Pair *pair = (Pair *) value;
    free(pair->word);
    free(pair);
}

/* Iterator that prints keys and values. */
void kv_printor (gpointer key, gpointer value, gpointer user_data)
{
    printf(user_data, key, *(gint *) value);
}


/* Iterator that adds key-value pairs to a sequence. */
void accumulator(gpointer key, gpointer value, gpointer user_data)
{
    GSequence *seq = (GSequence *) user_data;
    Pair *pair = g_new(Pair, 1);
    // make a copy of string to separate the hash and seq data structures
    pair->word = g_strdup((gchar *) key);
    pair->freq = *(gint *) value;

    g_sequence_insert_sorted(seq,
        (gpointer) pair,
        (GCompareDataFunc) compare_pair,
        NULL);

}

/* Iterator that frees key-value pairs. */
void hash_free(gpointer key, gpointer value, gpointer user_data)
{
    free(value);
    free(key);

}

/* Increments the frequency associated with key. */
void incr(GHashTable* hash, gchar *key)
{
    gint *val = (gint *) g_hash_table_lookup(hash, key);

    if (val == NULL) {
      // make copy of key
      gchar *key1 = g_strdup(key);
      gint *val1 = g_new(gint, 1);
      *val1 = 1;
      g_hash_table_insert(hash, key1, val1);
    } else {
      *val += 1;
    }
}

int main(int argc, char** argv)
{
    gchar *filename;

    // open the file
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "emma.txt";
    }

    FILE *fp = g_fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        exit(-10);
    }

    /* string array is a(two-L) NULL terminated array of pointers to
    (one-L) NUL terminated strings */
    gchar **array;
    gchar line[128];
    GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);

    // read lines from the file and build the hash table
    while (1) {
        gchar *res = fgets(line, sizeof(line), fp);
        if (res == NULL) break;
        // g_strsplit makes duplicates
        array = g_strsplit(line, " ", 0);
        for (int i=0; array[i] != NULL; i++) {
            incr(hash, array[i]);
        }
        g_strfreev(array);
    }
    fclose(fp);

    // print the hash table
    // g_hash_table_foreach(hash, (GHFunc) kv_printor, "Word %s freq %d\n");

    // iterate the hash table and build the sequence
    GSequence *seq = g_sequence_new(NULL);
    g_hash_table_foreach(hash, (GHFunc) accumulator, (gpointer) seq);

    // iterate the sequence and print the pairs
    g_sequence_foreach(seq, (GFunc) pair_printor, NULL);

    // try to free everything
    // first free every key-value pair in hash
    g_hash_table_foreach(hash, (GHFunc) hash_free, (gpointer) seq);
    // then free the hash table
    g_hash_table_destroy(hash);

    // free every Pair in sequence
    g_sequence_foreach(seq, (GFunc) pair_free, NULL);
    // then free the actual sequence
    g_sequence_free(seq);

    return 0;
}
