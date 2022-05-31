export interface GmodDto {
    visRelease: string;
    items: GmodNodeDto[];
    relations: GmodRelationDto[];
}

export interface GmodRelationDto {
    parent: string;
    child: string;
}

export interface GmodNodeDto {
    id?: string;
    category: string;
    type: string;
    code: string;
    name: string;
    commonName?: string | null;
    definition?: string | null;
    commonDefinition?: string | null;
    installSubstructure?: boolean | null;
    normalAssignmentNames?: { [key: string]: string };
    location?: string;
}
