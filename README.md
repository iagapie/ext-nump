# Matrix extension for PHP 7

[![Build Status](https://travis-ci.org/iagapie/ext-nump.svg?branch=master)](https://travis-ci.org/iagapie/ext-nump)

## Installation

```bash
phpize
./configure
make
make install
```

## Enabling the extension

You'll need to add `extension=nump.so` to your primary *php.ini* file.

```bash
# To see where .ini files are located
php -i | grep "\.ini"
```

---

You can also enable the extension temporarily using the command line:

```bash
php -d extension=nump.so
```

**Note**: Windows would use php_nump.dll instead. (NOT TESTED)

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

``` bash
composer install
composer test
```

## Nump Matrix class and functions

### Functions

```php
use Nump\Matrix;
use function Nump\{
    matrix_from,
    matrix_zeros,
    matrix_eye,
    matrix_rand,
    matrix_randn
};

/**
 * @param float[] $data
 * @return Matrix
 */
matrix_from(array $data) : Matrix

/**
 * @param int[] $shape Int or tuple of ints. Shape of the new matrix, e.g., (2, 3) or (2,).
 * @return Matrix
 */
matrix_zeros(array $shape) : Matrix

/**
 * @param int $m Number of rows in the output.
 * @param int $n Number of columns in the output. If 0, defaults to $m.
 * @param int $k Index of the diagonal: 0 (the default) refers to the main diagonal,
 *               a positive value refers to an upper diagonal,
 *               and a negative value to a lower diagonal.
 * @return Matrix
 */
matrix_eye(int $m, int $n = 0, int $k = 0) : Matrix

/**
 * @param int[] $shape Int or tuple of ints. Shape of the new matrix, e.g., (2, 3) or (2,).
 * @return Matrix
 */
matrix_rand(array $shape) : Matrix

/**
 * @param int[] $shape Int or tuple of ints. Shape of the new matrix, e.g., (2, 3) or (2,).
 * @return Matrix
 */
matrix_randn(array $shape) : Matrix

```

### Class synopsis

```php
Nump\Matrix implements Traversable, JsonSerializable, Countable {
    public __construct ( array $data )
    public value ( int|int[] $index ) : float
    public isset ( int|int[] $index ) : bool
    public pow ( double $powerValue ) : Matrix
    public exp ( void ) : Matrix
    public log ( void ) : Matrix
    public log2 ( void ) : Matrix
    public log10 ( void ) : Matrix
    public negative ( void ) : Matrix
    public transpose ( void ) : Matrix
    public inverse ( void ) : Matrix
    public determinant ( void ) : float
    public mean ( void ) : float
    public shape ( void ) : int[]
    public count ( void ) : int
    public isEmpty ( void ) : bool
    public isSquare ( void ) : bool
    public isSingular ( void ) : bool
    public toArray ( void ) : array
    public toFlatten ( void ) : float[]
    public jsonSerialize ( void ) : string
    public round ( [int $precision = 0 [, int $mode = PHP_ROUND_HALF_UP ]] ) : Matrix
    public apply ( callable $callback ) : Matrix
    public solve ( Matrix $b ) : Matrix
    public add ( float|float[]|Matrix $b ) : Matrix
    public sub ( float|float[]|Matrix $b ) : Matrix
    public mul ( float|float[]|Matrix $b ) : Matrix
    public div ( float|float[]|Matrix $b ) : Matrix
    public dot ( float|float[]|Matrix $b ) : Matrix
    public __toString ( void ) : string
}
```

## Example

```php
use Nump\Matrix as M;
use function Nump\{matrix_from,matrix_randn};

class NeuralNetwork
{
    private $w1;
    private $w2;
    private $z2;
    private $z3;

    public function __construct($inputSize = 2, $hiddenSize = 3, $outputSize = 1)
    {
        $this->w1 = matrix_randn([$inputSize, $hiddenSize]);
        $this->w2 = matrix_randn([$hiddenSize, $outputSize]);
    }

    public function predict(M $x): M
    {
        return $this->forward($x);
    }

    public function train(M $x, M $y, int $epochs = 15000, bool $verbose = true): void
    {
        if ($verbose) {
            printf("Training Input (scaled): \n%s\n", $x->__toString());
            printf("Training Output: \n%s\n", $y->__toString());
        }

        for ($i = 0; $i < $epochs; ++$i) {
            $o = $this->forward($x);
            $this->backward($x, $y, $o);

            if ($verbose) {
                printf("\n# %s\n", $i);
                printf("Predicted Output: \n%s\n", $o->__toString());
                $loss = $y->sub($o)->apply(function ($value) { return $value * $value; })->mean();
                printf("Loss: \n%s\n", $loss);
            }
        }
    }

    private function forward(M $x): M
    {
        $z = $x->dot($this->w1);
        $this->z2 = $this->sigmoid($z);
        $this->z3 = $this->z2->dot($this->w2);
        $o = $this->sigmoid($this->z3);
        return $o;
    }

    private function backward(M $x, M $y, M $o): void
    {
        $error = $y->sub($o);
        $delta = $error->mul($this->sigmoidPrime($o));
        $z2Error = $delta->dot($this->w2->transpose());
        $z2Delta = $z2Error->mul($this->sigmoidPrime($this->z2));
        $this->w1 = $this->w1->add($x->transpose()->dot($z2Delta));
        $this->w2 = $this->w2->add($this->z2->transpose()->dot($delta));
    }

    private function sigmoid(M $s): M
    {
        return $s->negative()->exp()->apply(function ($value) {
            return 1 / (1 + $value);
        });
    }

    private function sigmoidPrime(M $s): M
    {
        return $s->apply(function ($value) {
            return $value * (1 - $value);
        });
    }
}

// from http://php.net/manual/en/function.filesize.php
function formatBytes($bytes, $precision = 2) {
    $units = array('B', 'KB', 'MB', 'GB', 'TB');

    $bytes = max($bytes, 0);
    $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
    $pow = min($pow, count($units) - 1);

    $bytes /= pow(1024, $pow);

    return round($bytes, $precision) . ' ' . $units[$pow];
}

$start = microtime(true);

$x = matrix_from([[0.4, 0.9], [0.2, 0.5], [0.6, 0.6]]);
$y = matrix_from([[92], [86], [89]])->div(100);

$nn = new NeuralNetwork();
$nn->train($x, $y);

$xp = matrix_from([[1., 1.]]);
$predicted = $nn->predict($xp);

printf("\nPredicted data based on trained weights:\n");
printf("Input (scaled):\n%s\n", $xp->__toString());
printf("Output:\n%s\n", $predicted->__toString());

printf("\nTotal time: %s\r\nMemory Used (current): %s\r\nMemory Used (max): %s\n", round(microtime(true) - $start, 4), formatBytes(memory_get_usage()), formatBytes(memory_get_peak_usage()));
```

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
