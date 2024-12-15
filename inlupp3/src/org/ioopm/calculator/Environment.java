package org.ioopm.calculator;

import org.ioopm.calculator.ast.Variable;
import org.ioopm.calculator.parser.SymbolicExpression;
import java.util.HashMap;

/// An 'Environment' is a renaming of a hashmap containing variables and their expressions.
/// If (1 + 2) * 3 is assigned to the variable x, then x will be stored as a KEY and (1 + 2) * 3 as the VALUE in the hashmap
/// The .hashcode() function is overwritten within the 'Variable' module so that hashing is done with the variable name, and not the object itself.
public class Environment extends HashMap<Variable, SymbolicExpression> { }
