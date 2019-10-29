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

class SubTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            3,
            [[-5, -1, -6], [-4, -2, 0], [-1, -3, -4]],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [1, 2, 1],
            [[-3, 0, -4], [-2, -1, 2], [1, -2, -2]],
        ];

        yield [
            [1, 2, 1],
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[3, 0, 4], [2, 1, -2], [-1, 2, 2]],
        ];

        yield [
            [1, 2, 1],
            [2, 3, 4],
            [-1, -1, -3],
        ];

        yield [
            [1, 2, 1],
            6,
            [-5, -4, -5],
        ];

        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [[3, 4, 5], [12, 65, 2], [0, 1, 0]],
            [[-5, -2, -8], [-13, -64, 1], [2, -1, -1]],
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

        $actual = $m->sub($b)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
