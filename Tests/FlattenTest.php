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

class FlattenTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [[-2, 2, -3], [-1, 1, 3], [2, 0, -1]],
            [-2, 2, -3, -1, 1, 3, 2, 0, -1]
        ];

        yield [
            [-2, 2, -3, -1, 1, 3, 2, 0, -1],
            [-2, 2, -3, -1, 1, 3, 2, 0, -1]
        ];

        yield [[], []];
    }

    /**
     * @dataProvider provider
     *
     * @param array $matrix
     * @param array $expected
     */
    public function testMatrix(array $matrix, array $expected): void
    {
        $m = new Matrix($matrix);

        $actual = $m->toFlatten();

        $this->assertEquals($expected, $actual);
    }
}
