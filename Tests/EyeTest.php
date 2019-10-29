<?php

/**
 * Copyright © 2019. All rights reserved.
 *
 * @author Igor Agapie <igoragapie@gmail.com>
 */

declare(strict_types=1);

namespace Nump\Tests;

use function Nump\matrix_eye;
use PHPUnit\Framework\TestCase;

class EyeTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [1, 0, 0, [[1]]];
        yield [1, 3, 1, [[0, 1, 0]]];
        yield [3, 3, -1, [[0, 0, 0], [1, 0, 0], [0, 1, 0]]];
        yield [3, 3, 0, [[1, 0, 0], [0, 1, 0], [0, 0, 1]]];
        yield [4, 4, -6, [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]];
    }

    /**
     * @dataProvider provider
     *
     * @param int $m
     * @param int $n
     * @param int $k
     * @param array $expected
     */
    public function testMatrix(int $m, int $n, int $k, array $expected): void
    {
        $actual = matrix_eye($m, $n, $k)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
