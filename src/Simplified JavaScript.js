/**
 * Simplified JavaScript parser
 * written in Simplified JavaScript
 * Douglas Crockford, 2007
 */
 
/* {{{ *//* }}} */

var expression = function (rbp) {
    var left;
    var t = token;
    advance();
    left = t.nud();
    while (rbp < token.lbp) {
        t = token;
        advance();
        left = t.led(left);
    }
    return left;
}

/* {{{ *//* }}} */

var infix = function (id, bp, led) {
    var s = symbol(id, bp);
    s.led = led || function (left) {
        this.first = left;
        this.second = expression(bp);
        this.arity = "binary";
        return this;
    };
    return s;
}

infix("+", 60);
infix("-", 60);
infix("*", 70);
infix("/", 70);

/* {{{ *//* }}} */

infix("===", 50);
infix("!==", 50);
infix("<", 50);
infix("<=", 50);
infix(">", 50);
infix(">=", 50);

infix("?", 20, function (left) {
    this.first = left;
    this.second = expression(0);
    advance(":");
    this.third = expression(0);
    this.arity = "ternary";
    return this;
});

/* {{{ *//* }}} */

var prefix = function (id, nud) {
    var s = symbol(id);
    s.nud = nud || function ( ) {
        scope.reserve(this);
        this.first = expression(80);
        this.arity = "unary";
        return this;
    };
    return s;
}
prefix("-");
prefix("!");
prefix("typeof");

prefix("(", function ( ) {
    var e = expression(0);
    advance(")");
    return e;
});

/* {{{ *//* }}} */

var assignment = function (id) {
    return infixr(id, 10, function (left) {
        if (left.id !== "." && left.id !== "[" &&
            left.arity !== "name") {
            left.error("Bad lvalue.");
        }
        this.first = left;
        this.second = expression(9);
        this.assignment = true;
        this.arity = "binary";
        return this;
    });
};
assignment("=");
assignment("+=");
assignment("-=");

/* {{{ *//* }}} */

prefix("function", function ( ) {
    var a = [];
    scope = new_scope( );
    if (token.arity === "name") {
        scope.define(token);
        this.name = token.value;
        advance( );
    }
    advance("(");
    if (token.id !== ")") {
        while (true) {
            if (token.arity !== "name") {
                token.error("Expected a parameter name.");
            }
            scope.define(token);
            a.push(token);
            advance( );
            if (token.id !== ",") {
                break;
            }
            advance(",");
        }
    }
    this.first = a;
    advance(")");
    advance("{");
    this.second = statements( );
    advance("}");
    this.arity = "function";
    scope.pop( );
    return this;
});

/* {{{ *//* }}} */

prefix("[", function ( ) {
    var a = [];
    if (token.id !== "]") {
        while (true) {
            a.push(expression(0));
            if (token.id !== ",") {
                break;
            }
            advance(",");
        }
    }
    advance("]");
    this.first = a;
    this.arity = "unary";
    return this;
});

/**
 * Beautiful Code, O'Reilly, 2007
 */
