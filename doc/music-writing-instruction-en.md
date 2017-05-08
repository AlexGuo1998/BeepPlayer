# Instruction for Writing Music Scores

A piece of music score contains *notes*, *instructions* and *lyrics*. It is compatible with the `PLAY` command in `PC LOGO`.

## Notes

* There are 8 kinds of notes: `C`、`D`、`E`、`F`、`G`、`A`、`B`、`P`, in which `P` means pause.
* The number ahead of note means its duration. E.g. `4C` is a crochet(1/4 of a full note) C, `16E` is a semiquaver(1/16 of a full note) E.
  * The number can be a float. E.g. `1.6C` means 5/8 of a full C note.
  * If there's no number ahead of a note, the default duration, which can be modified with `L` command, will be used.
* `#`, `b` followed by a note can change its pitch. E.g. `C#`, `Db` is equal. `C##` is equal to `D`, but not recommended.
* `.` followed by a note can change its length(time) to 1.5*length. E.g. the time of `4C.` is equal to the time of `4C 8C`.

## Instructions

* There are some kinds of instructions: `O`、`T`、`L`、`S`、`H`、`R`、`[`、`]`. They are for changing octave, changing tempo, changing default length, changing staccato, changing pitch offset, reset and playing chords.
* `O`: Change octave
  * `O#` raises an octave, `Ob` reduces an octave.
  * `On` (0 <= n <= 7, n=integer) sets the octave to *n*.
  * The default value is `O3`.
* `T`: Set the tempo
  * `Tn` (30 <= n <= 1000, n = float) sets the tempo(how many crochets per minute) to *n*.
  * The default value is `T120`.
* `L`: Set the default duration
  * `Ln` (1 <= n <= 128, n = integer) sets the default duration to *n*. E.g. `L8 C D E` is equal to `8C 8D 8E`.
  * The default value is `L4`.
* `S`: Set the staccato
  * `Sn` (0 <= n <= 100, n = float) sets the staccato to *n* percent (*n*%). E.g. `S50` means half of the note sounds, `S0` means no gap between notes, and notes will connect (not on Windows), `S100` means no sound.
  * The default value is `S10`.
* `H`: Set pitch offset
  * `H#` raises a semitone, `Hb` reduces a semitone.
  * `Hn` (-12 <= n <= 12, n = integer)(float may be supported in the future) sets pitch offset to *n* semitone(s). E.g. `H1 C` is equal to `H0 C#`.
  * The default value is `H0`.
  * Use it properly can reduce the size of the file, BUT it is not compatible with  `PC LOGO`.
* `R`: Reset
  * Equals to `O3 T120 L4 S10 H0`.
  * `R` will be automatically applied when you load a new file.
* `[` and `]` is used to define a chord
  * Notes between `[` and `]` (except the last one) will be played quickly, to semulate a chord.
  * Those notes are 0.1s each. To make sure the total time is correct, the time of the last note is decresed.

## Lyrics

Lyrics are surrounded by `*`, and splitted by `|`. the string splitted by `|` is for one note.

A pair of `*` means a line of lyric, and *newline* will be automatically added. If new line of lyrics meet when there are words not used, those words will be discarded.

E.g. 1
```
*Twin|kle| twin|kle| lit|tle| star*
C C G G A A 2G
```
"Twin" -> first `C`, "kle" -> second `C`, ... , the last "star" -> `2G`。

E.g. 2
```
*今|年||の|夏||の|匂||い|*
8F G G# 8G C C 8C# D# F 8D# 8P
```
"年" -> `G`, null string -> `G#`. When playing, it seems that "年" -> `G G#`。

**Be careful**: pauses and notes in a chord will be regarded as common notes.

## Others

* Except for the `b` in `Ob`, `Hb` and `Db`, other *notes* and *instructions* are case insensitive.
  * But be careful, `db` is a flat D, while `DB` is `D` and `B`
* Words startes with `//` to the end of the line are regarded as comments.

## Last Words

The writer of this document is from China, and he speaks poor English.

If you are from English-speaking countries and you have some spare time, you can correct the mistakes in this documents. I will be very grateful.