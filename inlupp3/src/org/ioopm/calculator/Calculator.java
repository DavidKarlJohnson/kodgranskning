package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;
import java.io.IOException;
import java.util.Scanner;

public class Calculator {


    public static void main(String[] args) throws IOException {
        final CalculatorParser parser = new CalculatorParser();
        final Environment environment = new Environment();
        Scanner scanner = new Scanner(System.in);
        System.out.println("Welcome to the parser!");

        while(true) {
            System.out.print("Please enter an expression: ");
            String input = scanner.nextLine().toLowerCase();
            SymbolicExpression parsedString = parser.parse(input, environment);

            if(parsedString.isCommand()) {  // Command
                if(parsedString instanceof Quit) {
                    System.out.println("quitting...");
                    break;
                }
                else if (parsedString instanceof Vars) {
                    if(environment.isEmpty()) {
                        System.out.println("There exists no variables!");
                    }
                    else {
                        environment.forEach((key, value) -> System.out.println(key + " = " + value));
                    }
                }
                else if (parsedString instanceof Clear) {
                    environment.clear();
                    System.out.println("All variables cleared!");
                }
            }
            else {  // Parse and evaluate
                try {
                    SymbolicExpression ans = parsedString.eval(environment);
                    System.out.println(ans.toString());
                }
                catch (IllegalAssignmentException e) {
                    System.err.println("Illegal assignment");
                }
            }
        }
    }
}
