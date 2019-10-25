<?php

/**
 * Copyright © 2019. All rights reserved.
 *
 * @author Igor Agapie <igoragapie@gmail.com>
 */

declare(strict_types=1);

namespace Nump\Tests;

use Nump\Matrix;
use function Nump\matrix_from;
use function Nump\matrix_rand;
use function Nump\matrix_randn;
use PHPUnit\Framework\TestCase;

class MatrixTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [[[1, 0, 0], [0, 1, 0], [0, 0, 1]]];
        yield [[[1, 5, 0], [2, 4, 6], [5, 3, 2]]];
    }

    /**
     * @dataProvider provider
     *
     * @param array $data
     */
    public function testFrom(array $data): void
    {
        $matrix = matrix_from($data);

        $actual = $matrix->toArray();
        $expected = $data;

        $this->assertEquals($expected, $actual);
    }
    
    public function testRand(): void
    {
        $matrix = matrix_rand([3, 3]);

        $actual = $matrix->shape();
        $expected = [3, 3];

        $this->assertEquals($expected, $actual);
    }
    
    public function testRandn(): void
    {
        $matrix = matrix_randn([3, 3]);

        $actual = $matrix->shape();
        $expected = [3, 3];

        $this->assertEquals($expected, $actual);
    }
    
    public function testIsEmpty(): void
    {
        $matrix = new Matrix([]);

        $actual = $matrix->isEmpty();

        $this->assertTrue($actual);
    }

    /**
     * @dataProvider provider
     *
     * @param array $data
     */
    public function testSize(array $data): void
    {
        $matrix = new Matrix($data);

        $actual = $matrix->size();
        $expected = 9;

        $this->assertEquals($expected, $actual);
    }

    /**
     * @dataProvider provider
     *
     * @param array $data
     */
    public function testShape(array $data): void
    {
        $matrix = new Matrix($data);

        $actual = $matrix->shape();
        $expected = [3, 3];

        $this->assertEquals($expected, $actual);
    }

    /**
     * @dataProvider provider
     *
     * @param array $data
     */
    // public function testIsSquare(array $data): void
    // {
    //     $matrix = new Matrix($data);

    //     $actual = $matrix->isSquare();

    //     $this->assertTrue($actual);
    // }
    
    // public function testIsSingular(): void
    // {
    //     $data = [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]];
    //     $matrix = new Matrix($data);

    //     $actual = $matrix->isSingular();

    //     $this->assertTrue($actual);
    // }
    
    public function testApply(): void
    {
        $data = [[1, 5, 0], [2, 4, 6], [5, 3, 2]];
        $matrix = new Matrix($data);

        $actual = $matrix->apply(function ($item, $i) {
            return $item * $i;
        })->toArray();
        $expected = [[0, 5, 0], [6, 16, 30], [30, 21, 16]];

        $this->assertEquals($expected, $actual);
    }

    /**
     * @dataProvider provider
     *
     * @param array $data
     */
    // public function testToString(array $data): void
    // {
    //     $matrix = new Matrix($data);

    //     $actual = $matrix->__toString();

    //     $this->assertNotEmpty($actual);
    // }
}
