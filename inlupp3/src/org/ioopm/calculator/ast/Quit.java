package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Quit extends Command {
    private static final Quit theInstance = new Quit();


    private Quit() {}


    public static Quit instance() {
        return theInstance;
    }


    @Override
    public boolean isCommand() {
        return true;
    }


    @Override
    public SymbolicExpression eval(Environment environment) {
        return null;
    }
}
