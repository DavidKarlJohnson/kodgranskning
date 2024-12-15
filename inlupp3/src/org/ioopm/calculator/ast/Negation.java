package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Negation extends Unary {


    public Negation(SymbolicExpression operator) {
        super(operator);
    }


    public String getName() {
        return "neg";
    }


    public boolean equals(Object other) {
        if(other instanceof Negation) {
            if(this.value == ((Negation) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression val = this.value.eval(environment);
        if (val.isConstant()) {
            return new Constant((-val.getValue()));
        } else {
            return new Negation(val);
        }
    }
}
