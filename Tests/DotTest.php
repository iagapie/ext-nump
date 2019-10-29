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

class DotTest extends TestCase
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
            [-1, 4, 1],
        ];

        yield [
            [1, 2, 1],
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [-2, 4, 2],
        ];

        yield [
            [1, 2, 1],
            [2, 3, 4],
            [12],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[3, 4, 5], [12, 65, 2], [0, 1, 0]],
            [[18, 119, -6], [9, 64, -3], [6, 7, 10]],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[3, 4], [12, 65], [0, 1]],
            [[18, 119], [9, 64], [6, 7]]
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

        $actual = $m->dot($b)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
