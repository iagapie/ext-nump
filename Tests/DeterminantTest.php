<?php

/**
 * Copyright © 2019. All rights reserved.
 *
 * @author Igor Agapie <igoragapie@gmail.com>
 */

declare(strict_types=1);

namespace Nump\Tests;

use Nump\Matrix;
use PHPUnit\Framework\TestCase;

class DeterminantTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [[[-2, 2, -3], [-1, 1, 3], [2, 0, -1]], 18];
        yield [[[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]], 0];
        yield [[[1, 2, 3, 4], [8, 5, 6, 7], [9, 12, 10, 11], [13, 14, 16, 15]], -348];
        yield [[[1, 2, 3, 4, 1], [8, 5, 6, 7, 2], [9, 12, 10, 11, 3], [13, 14, 16, 15, 4], [10, 8, 6, 4, 2]], -240];
    }

    /**
     * @dataProvider provider
     *
     * @param array $matrix
     * @param int $expected
     */
    public function testMatrix(array $matrix, int $expected): void
    {
        $m = new Matrix($matrix);

        $actual = $m->determinant();

        $this->assertEquals($expected, $actual);
    }
}
