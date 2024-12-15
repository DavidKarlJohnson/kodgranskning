package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Exp extends Unary {

    public Exp(SymbolicExpression operator) {
        super(operator);
    }


    public String getName() {
        return "exp";
    }


    public boolean equals(Object other) {
        if(other instanceof Exp) {
            if(this.value == ((Exp) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression val = this.value.eval(environment);
        if (val.isConstant()) {
            return new Constant(Math.exp(val.getValue()));
        } else {
            return new Exp(val);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}
