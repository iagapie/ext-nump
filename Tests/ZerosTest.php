<?php

/**
 * Copyright © 2019. All rights reserved.
 *
 * @author Igor Agapie <igoragapie@gmail.com>
 */

declare(strict_types=1);

namespace Nump\Tests;

use function Nump\matrix_zeros;
use PHPUnit\Framework\TestCase;

class ZerosTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [3, 3],
            [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
        ];

        yield [
            [3, 2],
            [[0, 0], [0, 0], [0, 0]],
        ];

        yield [
            [1, 3],
            [[0, 0, 0]],
        ];

        yield [
            [3],
            [0, 0, 0],
        ];
    }

    /**
     * @dataProvider provider
     *
     * @param array $shape
     * @param array $expected
     */
    public function testMatrix(array $shape, array $expected): void
    {
        $actual = matrix_zeros($shape)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
