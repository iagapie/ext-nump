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

class MulTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            3,
            [[-6, 6, -9], [-3, 3, 9], [6, 0, -3]],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [1, 2, 1],
            [[-2, 4, -3], [-1, 2, 3], [2, 0, -1]],
        ];

        yield [
            [1, 2, 1],
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[-2, 4, -3], [-1, 2, 3], [2, 0, -1]],
        ];

        yield [
            [1, 2, 1],
            [2, 3, 4],
            [2, 6, 4],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[3, 4, 5], [12, 65, 2], [0, 1, 0]],
            [[-6, 8, -15], [-12, 65, 6], [0, 0, 0]],
        ];
    }

    /**
     * @dataProvider provider
     *
     * @param array $matrix
     * @param mixed $b
     * @param array $expected
     */
    public function testMatrix(array $matrix, $b, array $expected): void
    {
        $m = new Matrix($matrix);

        $actual = $m->mul($b)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
