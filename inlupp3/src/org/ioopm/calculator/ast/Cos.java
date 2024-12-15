package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Cos extends Unary {

    public Cos(SymbolicExpression operator) {
        super(operator);
    }


    public String getName() {
        return "cos";
    }


    public boolean equals(Object other) {
        if(other instanceof Cos) {
            if(this.value == ((Cos) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression val = this.value.eval(environment);
        if (val.isConstant()) {
            return new Constant(Math.cos(val.getValue()));
        } else {
            return new Cos(val);
        }
    }

    @Override
    public String toString() {
        return super.toString();
    }
}
