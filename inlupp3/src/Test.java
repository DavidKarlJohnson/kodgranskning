import org.ioopm.calculator.Environment;
import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;

public class Test {


    public static void testPrinting(String expected, SymbolicExpression e) {
        if (expected.equals("" + e)) {
            System.out.println("PASSED!   " + e);
        } else {
            System.out.println("ERROR!   Expected '" + expected + "'   |   '" + e + "'");
        }
    }


    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression actual, Environment environment) throws IllegalAssignmentException {
        SymbolicExpression calculation = actual.eval(environment);
        if (calculation.equals(expected)) {
            System.out.println("PASSED!  Input: " + actual + "  |  Expected: " + expected + "  |  Got: " + calculation);
        } else {
            System.out.println("ERROR!  Input: " + actual + "  |  Expected: " + expected + "  |  Got: " + calculation);
        }
    }


    public static void main(String[] args) throws IllegalAssignmentException {

        // Neg(10) = -10
        testEvaluating(new Constant(-10), new Negation(new Constant(10)), new Environment());
        // Neg(-10) = 10
        testEvaluating(new Constant(10), new Negation(new Constant(-10)), new Environment());

        // Constant(5) = 5
        testEvaluating(new Constant(5), new Constant(5), new Environment());


        // Looking up "x" should return "x"
        // Then, after assigning 5 to x, looking up "x" should return 5
        Environment environment2 = new Environment();
        testEvaluating(new Variable("x"), new Variable("x"), environment2);
        testEvaluating(new Constant(5), new Assignment(new Constant(5), new Variable("x")), environment2);
        testEvaluating(new Constant(5), new Variable("x"), environment2);


        // 7+3 = x  <=>  10 = x
        // Then... reassign:  50+22 = x  <=>  72 = x
        Environment environment1 = new Environment();
        Variable varX = new Variable("x");
        testEvaluating(new Constant(10), new Assignment(new Addition(new Constant(7), new Constant(3)), varX), environment1);
        testEvaluating(new Constant(10), varX, environment1);
        testEvaluating(new Constant(72), new Assignment(new Addition(new Constant(50), new Constant(22)), varX), environment1);
        testEvaluating(new Constant(72), varX, environment1);


        // Testing the: .toString() functions
        testPrinting("sin(1.0)", new Sin(new Constant(1)));
        testPrinting("cos(1.0)", new Cos(new Constant(1)));
        testPrinting("exp(1.0)", new Exp(new Constant(1)));
        testPrinting("log(1.0)", new Log(new Constant(1)));
        testPrinting("neg(1.0)", new Negation(new Constant(1)));

        
        // Sin(1) = 0.8414709848078965
        testEvaluating(new Constant(0.8414709848078965), new Sin(new Constant(1)), new Environment());
        // Sin(Sin(1)) = 0.7456241416655579
        testEvaluating(new Constant(0.7456241416655579), new Sin(new Sin(new Constant(1))), new Environment());
        // Cos(1) = 0.5403023058681398
        testEvaluating(new Constant(0.5403023058681398), new Cos(new Constant(1)), new Environment());
        // Cos(Cos(1)) = 0.8575532158463934
        testEvaluating(new Constant(0.8575532158463934), new Cos(new Cos(new Constant(1))), new Environment());
        // Sin(Cos(1)) = 0.8575532158463934
        testEvaluating(new Constant(0.5143952585235492), new Sin(new Cos(new Constant(1))), new Environment());
        // Cos(Sin(1)) = 0.6663667453928805
        testEvaluating(new Constant(0.6663667453928805), new Cos(new Sin(new Constant(1))), new Environment());


        // Log(10) = 1
        testEvaluating(new Constant(1), new Log(new Constant(10)), new Environment());
        // Log(Log(1,000,000)) = Log(6) = 0.7781512503836436
        testEvaluating(new Constant(0.7781512503836436), new Log(new Log(new Constant(1000000))), new Environment());


        // Exp(10) = 22026.465794806718
        testEvaluating(new Constant(22026.465794806718), new Exp(new Constant(10)), new Environment());
        // Exp(Exp(1)) = 15.154262241479262
        testEvaluating(new Constant(15.154262241479262), new Exp(new Exp(new Constant(1))), new Environment());


        // 5 + 37 = 42
        testEvaluating(new Constant(42), new Addition(new Constant(5), new Constant(37)), new Environment());
        // 5 * 6 = 30
        testEvaluating(new Constant(30), new Multiplication(new Constant(5), new Constant(6)), new Environment());
        // (5 * 6) * 10 = 300
        testEvaluating(new Constant(300), new Multiplication(new Multiplication(new Constant(5), new Constant(6)), new Constant(10)), new Environment());
        // ((10-5) - 2) - (15 - 10) = -2
        testEvaluating(new Constant(-2), new Subtraction(new Subtraction(new Subtraction(new Constant(10), new Constant(5)), new Constant(2)),
                new Subtraction(new Constant(15), new Constant(10))), new Environment());
        // ((100 / 10) / 2) / (25 / 5) = 1
        testEvaluating(new Constant(1), new Division(new Division(new Division(new Constant(100), new Constant(10)), new Constant(2)),
                new Division(new Constant(25), new Constant(5))), new Environment());
        // (10 + 10) * 5 = 100
        testEvaluating(new Constant(100), new Multiplication(new Addition(new Constant(10), new Constant(10)), new Constant(5)), new Environment());
        // (10 * 10) / 10 = 10
        testEvaluating(new Constant(10), new Division(new Multiplication(new Constant(10), new Constant(10)), new Constant(10)), new Environment());
        // 110 - 120 + 5 = 5
        testEvaluating(new Constant(-5), new Addition(new Subtraction(new Constant(110), new Constant(120)), new Constant(5)), new Environment());
        // 10-(-40) = 50
        testEvaluating(new Constant(50), new Subtraction(new Constant(10), new Constant(-40)), new Environment());
        // (10+20) + (30+40) = 100
        //testEvaluating(new Constant(100), new Addition(new Addition(new Constant(10), new Constant(20)), new Addition(new Constant(30), new Constant(40))), new Environment());
        // (10+20) + (30-40) = 20
        testEvaluating(new Constant(20), new Addition(new Addition(new Constant(10), new Constant(20)), new Subtraction(new Constant(30), new Constant(40))), new Environment());
    }
}
