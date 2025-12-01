import fs from "fs/promises";

type DirLetter = 'L' | 'R';

class Direction {
    #sign: 1 | -1;
    #magnitude: number;

    constructor(ltr: DirLetter, magnitude: number) {
        this.#sign = ltr === 'L' ? -1 : +1;
        this.#magnitude = magnitude;
    }

    toVector(): number {
        return this.#sign * this.#magnitude;
    }
}

class ComboLock {
    static readonly #range = 100;
    static readonly #defDial = 50;

    #zeroCounter = 0;
    #dialAt = ComboLock.#defDial;

    get password(): number {
        return this.#zeroCounter;
    }

    changeDialBy(dir: Direction): void {
        const vector = dir.toVector();
        this.#dialAt += vector;
        if (this.#dialAt % ComboLock.#range === 0) {
            ++this.#zeroCounter;
        }
    }

    changeDialBy2(dir: Direction): void {
        const cycleTicks = ComboLock.#range;
        const oldPos = this.#dialAt;
        const delta = dir.toVector();

        let min = 0;
        let max = 0;
        if (delta > 0) {
            min = Math.ceil((oldPos + 1) / cycleTicks);
            max = Math.floor((oldPos + delta) / cycleTicks);
        } else if (delta < 0) {
            min = Math.ceil((oldPos + delta) / cycleTicks);
            max = Math.floor((oldPos - 1) / cycleTicks);
        }

        const revolutions = Math.max(0, max - min + 1);
        this.#zeroCounter += revolutions;
        const newPos = oldPos + delta;
        this.#dialAt = ((newPos % cycleTicks) + cycleTicks) % cycleTicks;
    }
}

(async () => {
    const file = await fs.readFile("./day1/input.txt");
    const inputs = file.toString().split("\n");
    const vectors = inputs.map(input => {
        const ltr = input.substring(0, 1) as DirLetter;
        const magnitude = parseInt(input.substring(1));
        return new Direction(ltr, magnitude);
    });

    const lock = new ComboLock();
    const lock2 = new ComboLock();
    for (const vector of vectors) {
        lock.changeDialBy(vector);   // Part 1
        lock2.changeDialBy2(vector); // Part 2
    }

    console.log(lock.password);
    console.log(lock2.password);
})();