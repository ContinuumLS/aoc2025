import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Stack;

public class Day3 {
    public static void main(String[] args) {
        File file;
        Scanner stream;
        try {
            file = new File("./input.txt");
            stream = new Scanner(file);
        } catch (FileNotFoundException e) {
            System.err.println("Can't open input.");
            return;
        }
        ArrayList<String> banks = new ArrayList<>();
        while (stream.hasNextLine()) {
            banks.add(stream.nextLine());
        }
        part1(banks);
        part2(banks);
        stream.close();
    }

    private static void part1(ArrayList<String> banks) {
        int totalJoltage = 0;
        for (String bank : banks) {
            int joltage = 0;
            for (int i = 0; i < bank.length(); ++i) {
                int tens = charToInt(bank.charAt(i)) * 10;
                for (int j = i + 1; j < bank.length(); ++j) {
                    int ones = charToInt(bank.charAt(j));
                    int newJoltage = tens + ones;
                    if (newJoltage > joltage) {
                        joltage = newJoltage;
                    }
                }
            }
            totalJoltage += joltage;
        }
        System.out.println(totalJoltage);
    }

    private static void part2(ArrayList<String> banks) {
        long totalJoltage = 0l;
        final int numDigits = 12;
        for (String bank : banks) {
            final int bankSize = bank.length();
            Stack<Integer> joltageStack = new Stack<>();
            // Loop Invariant: All digits in the stack are in descending order.
            for (int i = 0; i < bankSize; ++i) {
                int digit = charToInt(bank.charAt(i));
                while (!joltageStack.empty()
                    && digit > joltageStack.peek()
                    && joltageStack.size() + (bankSize - i) > numDigits) {
                    joltageStack.pop();
                }
                if (joltageStack.size() < numDigits) {
                    joltageStack.push(digit);
                }
            }

            int[] digits = new int[numDigits];
            for (int i = numDigits - 1; i >= 0; --i) {
                digits[i] = joltageStack.pop();
            }

            long joltage = 0l;
            for (int d : digits) {
                joltage = joltage * 10 + d;
            }
            totalJoltage += joltage;
        }
        System.out.println(totalJoltage);
    }

    private static int charToInt(char c) {
        return c - '0';
    }
}