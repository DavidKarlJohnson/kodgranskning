package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Sin extends Unary {


    public Sin(SymbolicExpression operator) {
        super(operator);
    }


    public String getName() {
        return "sin";
    }


    public boolean equals(Object other) {
        if(other instanceof Sin) {
            if(this.value == ((Sin) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression val = this.value.eval(environment);
        if (val.isConstant()) {
            return new Constant(Math.sin(val.getValue()));
        } else {
            return new Sin(val);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}
