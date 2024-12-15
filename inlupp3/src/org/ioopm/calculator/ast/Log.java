package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Log extends Unary {


    public Log(SymbolicExpression operator) {
        super(operator);
    }


    public String getName() {
        return "log";
    }


    public boolean equals(Object other) {
        if(other instanceof Log) {
            if(this.value == ((Log) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression val = this.value.eval(environment);
        if (val.isConstant()) {
            return new Constant(Math.log10(val.getValue()));
        } else {
            return new Log(val);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}
