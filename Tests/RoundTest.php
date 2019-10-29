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

class RoundTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [
                [2.50000000e-01, -9.09090909e-02, 2.27272727e-02],
                [-8.33333333e-02, 1.81818182e-01, -1.28787879e-01],
                [2.01858732e-18, -9.09090909e-02, 2.72727273e-01],
            ],
            3,
            [[0.25, -0.091, 0.023], [-0.083, 0.182, -0.129], [0, -0.091, 0.273]],
        ];

        yield [
            [
                [0.05041302, 0.22125276, 0.60900819, 0.7827549],
                [0.72885331, 0.00965047, 0.03653146, 0.42360486],
                [0.08119958, 0.61019517, 0.84375958, 0.04340501],
                [0.326979, 0.88669279, 0.76185049, 0.46199796],
            ],
            4,
            [
                [0.0504, 0.2213, 0.609, 0.7828],
                [0.7289, 0.0097, 0.0365, 0.4236],
                [0.0812, 0.6102, 0.8438, 0.0434],
                [0.327, 0.8867, 0.7619, 0.462],
            ],
        ];
    }

    /**
     * @dataProvider provider
     *
     * @param array $matrix
     * @param int $precision
     * @param array $expected
     */
    public function testMatrix(array $matrix, int $precision, array $expected): void
    {
        $m = new Matrix($matrix);

        $actual = $m->round($precision)->toArray();

        $this->assertEquals($expected, $actual);
    }
}
