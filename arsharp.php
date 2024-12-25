<?php

class ArSharpInterpreter {
    private $code;
    private $variables = [];
    private $constants = [];
    private $grandValues = [];

    public function __construct($code) {
        $this->code = $code;
    }

    public function execute() {
        $lines = explode("\n", $this->code);
        $inComment = false;

        foreach ($lines as $line) {
            // Remove comments
            if (strpos($line, "//") !== false) {
                $line = substr($line, 0, strpos($line, "//"));
            }
            if (strpos($line, "s;") !== false) {
                continue; // Skip comments with "s;"
            }

            // Handle variable declarations
            if (strpos($line, "var{") !== false) {
                $parts = explode("=", $line);
                $varName = trim(substr($parts[0], 4, -1));
                if (isset($parts[1])) {
                    $this->variables[$varName] = $this->evaluateExpression(trim($parts[1]));
                } else {
                    $this->variables[$varName] = null;
                }
                continue;
            }

            // Handle constant declarations
            if (strpos($line, "const{") !== false) {
                $parts = explode("=", $line);
                $constName = trim(substr($parts[0], 5, -1));
                if (isset($parts[1])) {
                    $this->constants[$constName] = $this->evaluateExpression(trim($parts[1]));
                } else {
                    $this->constants[$constName] = null;
                }
                continue;
            }

            // Handle grand value declarations
            if (strpos($line, "g[") !== false) {
                $parts = explode("=", $line);
                $gName = trim(substr($parts[0], 2, -1));
                if (isset($parts[1])) {
                    $this->grandValues[$gName] = $this->evaluateExpression(trim($parts[1]));
                } else {
                    $this->grandValues[$gName] = null;
                }
                continue;
            }

            // Handle print statements
            if (strpos($line, "cout::") !== false) {
                $output = substr($line, 6, -7); // Extract the string to be printed
                echo $this->evaluateExpression($output) . "\n";
                continue;
            }

            // Handle other expressions (e.g., assignments, calculations) 
            // (This part needs to be more robust)
            // ...

        }
    }

    private function evaluateExpression($expression) {
        // Basic expression evaluation (can be expanded significantly)
        if (is_numeric($expression)) {
            return floatval($expression);
        }

        // Handle variables and constants
        if (strpos($expression, "var{") !== false) {
            $varName = trim(substr($expression, 4, -1));
            if (isset($this->variables[$varName])) {
                return $this->variables[$varName];
            }
        } elseif (strpos($expression, "const{") !== false) {
            $constName = trim(substr($expression, 5, -1));
            if (isset($this->constants[$constName])) {
                return $this->constants[$constName];
            }
        }

        // Handle grand values
        if (strpos($expression, "g[") !== false) {
            $gName = trim(substr($expression, 2, -1));
            if (isset($this->grandValues[$gName])) {
                return $this->grandValues[$gName];
            }
        }

        // Handle basic operators
        if (strpos($expression, "+") !== false) {
            $parts = explode("+", $expression);
            return $this->evaluateExpression($parts[0]) + $this->evaluateExpression($parts[1]);
        }
        if (strpos($expression, "-") !== false) {
            $parts = explode("-", $expression);
            return $this->evaluateExpression($parts[0]) - $this->evaluateExpression($parts[1]);
        }
        if (strpos($expression, "*") !== false) {
            $parts = explode("*", $expression);
            return $this->evaluateExpression($parts[0]) * $this->evaluateExpression($parts[1]);
        }
        if (strpos($expression, "/") !== false) {
            $parts = explode("/", $expression);
            return $this->evaluateExpression($parts[0]) / $this->evaluateExpression($parts[1]);
        }
        if (strpos($expression, "**") !== false) {
            $parts = explode("**", $expression);
            return pow($this->evaluateExpression($parts[0]), $this->evaluateExpression($parts[1]));
        }
        if (strpos($expression, "%") !== false) {
            $parts = explode("%", $expression);
            return $this->evaluateExpression($parts[0]) % $this->evaluateExpression($parts[1]);
        }

        // Handle other cases (e.g., function calls, more complex expressions)
        // ...

        return $expression; // Return the expression as-is if not handled
    }
}


?>