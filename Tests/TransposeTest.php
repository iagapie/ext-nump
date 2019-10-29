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

class TransposeTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [
                [0.05041302, 0.22125276, 0.60900819, 0.7827549],
                [0.72885331, 0.00965047, 0.03653146, 0.42360486],
                [0.08119958, 0.61019517, 0.84375958, 0.04340501],
                [0.326979, 0.88669279, 0.76185049, 0.46199796],
            ],
            [
                [0.05041302, 0.72885331, 0.08119958, 0.326979],
                [0.22125276, 0.00965047, 0.61019517, 0.88669279],
                [0.60900819, 0.03653146, 0.84375958, 0.76185049],
                [0.7827549, 0.42360486, 0.04340501, 0.46199796],
            ],
        ];

        yield [
            [0.05041302, 0.22125276, 0.60900819, 0.7827549],
            [0.05041302, 0.22125276, 0.60900819, 0.7827549]
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

        $actual = $m->transpose()->toArray();

        $this->assertEquals($expected, $actual);
    }
}
